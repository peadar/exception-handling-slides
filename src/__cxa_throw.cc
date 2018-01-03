extern "C" void
__cxxabiv1::__cxa_throw (void *obj, std::type_info *tinfo,
                         void (_GLIBCXX_CDTOR_CALLABI *dest) (void *))
{
  PROBE2 (throw, obj, tinfo);

  __cxa_eh_globals *globals = __cxa_get_globals ();
  globals->uncaughtExceptions += 1;

  // Definitely a primary.
  __cxa_refcounted_exception *header
    = __get_refcounted_exception_header_from_obj (obj);
  header->referenceCount = 1;
  header->exc.exceptionType = tinfo;
  header->exc.exceptionDestructor = dest;
  header->exc.unexpectedHandler = std::get_unexpected ();
  header->exc.terminateHandler = std::get_terminate ();
  __GXX_INIT_PRIMARY_EXCEPTION_CLASS(header->exc.unwindHeader.exception_class);
  header->exc.unwindHeader.exception_cleanup = __gxx_exception_cleanup;

#ifdef __USING_SJLJ_EXCEPTIONS__
  _Unwind_SjLj_RaiseException (&header->exc.unwindHeader);
#else
  _Unwind_RaiseException (&header->exc.unwindHeader);
#endif

  // Some sort of unwinding error.  Note that terminate is a handler.
  __cxa_begin_catch (&header->exc.unwindHeader);
  std::terminate ();
}

