#define PERSONALITY_FUNCTION	__gxx_personality_v0
_Unwind_Reason_Code
PERSONALITY_FUNCTION (int version,
		      _Unwind_Action actions,
		      _Unwind_Exception_Class exception_class,
		      struct _Unwind_Exception *ue_header,
		      struct _Unwind_Context *context)
{
  enum found_handler_type
  {
    found_nothing,
    found_terminate,
    found_cleanup,
    found_handler
  } found_type;

  lsda_header_info info;
  const unsigned char *language_specific_data;
  const unsigned char *action_record;
  const unsigned char *p;
  _Unwind_Ptr landing_pad, ip;
  int handler_switch_value;
  void* thrown_ptr = 0;
  bool foreign_exception;
  int ip_before_insn = 0;

  __cxa_exception* xh = __get_exception_header_from_ue(ue_header);

  // Interface version check.
  if (version != 1)
    return _URC_FATAL_PHASE1_ERROR;
  foreign_exception = !__is_gxx_exception_class(exception_class);

  // Shortcut for phase 2 found handler for domestic exception.
  if (actions == (_UA_CLEANUP_PHASE | _UA_HANDLER_FRAME)
      && !foreign_exception)
    {
      restore_caught_exception(ue_header, handler_switch_value,
			       language_specific_data, landing_pad);
      found_type = (landing_pad == 0 ? found_terminate : found_handler);
      goto install_context;
    }

  language_specific_data = (const unsigned char *)
    _Unwind_GetLanguageSpecificData (context);

  // If no LSDA, then there are no handlers or cleanups.
  if (! language_specific_data)
    CONTINUE_UNWINDING;

  // Parse the LSDA header.
  p = parse_lsda_header (context, language_specific_data, &info);
  info.ttype_base = base_of_encoded_value (info.ttype_encoding, context);
#ifdef _GLIBCXX_HAVE_GETIPINFO
  ip = _Unwind_GetIPInfo (context, &ip_before_insn);
#else
  ip = _Unwind_GetIP (context);
#endif
  if (! ip_before_insn)
    --ip;
  landing_pad = 0;
  action_record = 0;
  handler_switch_value = 0;

#ifdef __USING_SJLJ_EXCEPTIONS__
  // The given "IP" is an index into the call-site table, with two
  // exceptions -- -1 means no-action, and 0 means terminate.  But
  // since we're using uleb128 values, we've not got random access
  // to the array.
  if ((int) ip < 0)
    return _URC_CONTINUE_UNWIND;
  else if (ip == 0)
    {
      // Fall through to set found_terminate.
    }
  else
    {
      _uleb128_t cs_lp, cs_action;
      do
	{
	  p = read_uleb128 (p, &cs_lp);
	  p = read_uleb128 (p, &cs_action);
	}
      while (--ip);

      // Can never have null landing pad for sjlj -- that would have
      // been indicated by a -1 call site index.
      landing_pad = cs_lp + 1;
      if (cs_action)
	action_record = info.action_table + cs_action - 1;
      goto found_something;
    }
#else
  // Search the call-site table for the action associated with this IP.
  while (p < info.action_table)
    {
      _Unwind_Ptr cs_start, cs_len, cs_lp;
      _uleb128_t cs_action;

      // Note that all call-site encodings are "absolute" displacements.
      p = read_encoded_value (0, info.call_site_encoding, p, &cs_start);
      p = read_encoded_value (0, info.call_site_encoding, p, &cs_len);
      p = read_encoded_value (0, info.call_site_encoding, p, &cs_lp);
      p = read_uleb128 (p, &cs_action);

      // The table is sorted, so if we've passed the ip, stop.
      if (ip < info.Start + cs_start)
	p = info.action_table;
      else if (ip < info.Start + cs_start + cs_len)
	{
	  if (cs_lp)
	    landing_pad = info.LPStart + cs_lp;
	  if (cs_action)
	    action_record = info.action_table + cs_action - 1;
	  goto found_something;
	}
    }
#endif // __USING_SJLJ_EXCEPTIONS__

  // If ip is not present in the table, call terminate.  This is for
  // a destructor inside a cleanup, or a library routine the compiler
  // was not expecting to throw.
  found_type = found_terminate;
  goto do_something;

 found_something:
  if (landing_pad == 0)
    {
      // If ip is present, and has a null landing pad, there are
      // no cleanups or handlers to be run.
      found_type = found_nothing;
    }
  else if (action_record == 0)
    {
      // If ip is present, has a non-null landing pad, and a null
      // action table offset, then there are only cleanups present.
      // Cleanups use a zero switch value, as set above.
      found_type = found_cleanup;
    }
  else
    {
      // Otherwise we have a catch handler or exception specification.

      _sleb128_t ar_filter, ar_disp;
      const std::type_info* catch_type;
      _throw_typet* throw_type;
      bool saw_cleanup = false;
      bool saw_handler = false;

#if __cpp_rtti
      // During forced unwinding, match a magic exception type.
      if (actions & _UA_FORCE_UNWIND)
	{
	  throw_type = &typeid(abi::__forced_unwind);
	}
      // With a foreign exception class, there's no exception type.
      // ??? What to do about GNU Java and GNU Ada exceptions?
      else if (foreign_exception)
	{
	  throw_type = &typeid(abi::__foreign_exception);
	}
      else
#endif
        {
          thrown_ptr = __get_object_from_ue (ue_header);
          throw_type = __get_exception_header_from_obj
            (thrown_ptr)->exceptionType;
        }

      while (1)
	{
	  p = action_record;
	  p = read_sleb128 (p, &ar_filter);
	  read_sleb128 (p, &ar_disp);

	  if (ar_filter == 0)
	    {
	      // Zero filter values are cleanups.
	      saw_cleanup = true;
	    }
	  else if (ar_filter > 0)
	    {
	      // Positive filter values are handlers.
	      catch_type = get_ttype_entry (&info, ar_filter);

	      // Null catch type is a catch-all handler; we can catch foreign
	      // exceptions with this.  Otherwise we must match types.
	      if (! catch_type
		  || (throw_type
		      && get_adjusted_ptr (catch_type, throw_type,
					   &thrown_ptr)))
		{
		  saw_handler = true;
		  break;
		}
	    }
	  else
	    {
	      // Negative filter values are exception specifications.
	      // ??? How do foreign exceptions fit in?  As far as I can
	      // see we can't match because there's no __cxa_exception
	      // object to stuff bits in for __cxa_call_unexpected to use.
	      // Allow them iff the exception spec is non-empty.  I.e.
	      // a throw() specification results in __unexpected.
	      if ((throw_type
		   && !(actions & _UA_FORCE_UNWIND)
		   && !foreign_exception)
		  ? ! check_exception_spec (&info, throw_type, thrown_ptr,
					    ar_filter)
		  : empty_exception_spec (&info, ar_filter))
		{
		  saw_handler = true;
		  break;
		}
	    }

	  if (ar_disp == 0)
	    break;
	  action_record = p + ar_disp;
	}

      if (saw_handler)
	{
	  handler_switch_value = ar_filter;
	  found_type = found_handler;
	}
      else
	found_type = (saw_cleanup ? found_cleanup : found_nothing);
    }

 do_something:
   if (found_type == found_nothing)
     CONTINUE_UNWINDING;

  if (actions & _UA_SEARCH_PHASE)
    {
      if (found_type == found_cleanup)
	CONTINUE_UNWINDING;

      // For domestic exceptions, we cache data from phase 1 for phase 2.
      if (!foreign_exception)
        {
	  save_caught_exception(ue_header, context, thrown_ptr,
				handler_switch_value, language_specific_data,
				landing_pad, action_record);
	}
      return _URC_HANDLER_FOUND;
    }

 install_context:
  
  // We can't use any of the cxa routines with foreign exceptions,
  // because they all expect ue_header to be a struct __cxa_exception.
  // So in that case, call terminate or unexpected directly.
  if ((actions & _UA_FORCE_UNWIND)
      || foreign_exception)
    {
      if (found_type == found_terminate)
	std::terminate ();
      else if (handler_switch_value < 0)
	{
	  __try 
	    { std::unexpected (); } 
	  __catch(...) 
	    { std::terminate (); }
	}
    }
  else
    {
      if (found_type == found_terminate)
	__cxa_call_terminate(ue_header);

      // Cache the TType base value for __cxa_call_unexpected, as we won't
      // have an _Unwind_Context then.
      if (handler_switch_value < 0)
	{
	  parse_lsda_header (context, language_specific_data, &info);
	  info.ttype_base = base_of_encoded_value (info.ttype_encoding,
						   context);

	  xh->catchTemp = base_of_encoded_value (info.ttype_encoding, context);
	}
    }

  /* For targets with pointers smaller than the word size, we must extend the
     pointer, and this extension is target dependent.  */
  _Unwind_SetGR (context, __builtin_eh_return_data_regno (0),
		 __builtin_extend_pointer (ue_header));
  _Unwind_SetGR (context, __builtin_eh_return_data_regno (1),
		 handler_switch_value);
  _Unwind_SetIP (context, landing_pad);
  return _URC_INSTALL_CONTEXT;
}

