g__Fv:
.LBB9:
	pushl %ebp
	movl %esp,%ebp
	subl $44,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
.stabn 68,0,11,.LM8-g__Fv
.LM8:
	call __get_eh_context
	movl %eax,%edi
.stabn 68,0,12,.LM9-g__Fv
.LM9:
	addl $-12,%esp
	leal -1(%ebp),%esi
	pushl %esi
	call __7CleanMe
	movl 4(%edi),%edx
	movl 4(%edx),%eax
	movl %eax,-16(%ebp)
	movl $_$_7CleanMe,-12(%ebp)
	movl %esi,-8(%ebp)
	leal -16(%ebp),%eax
	movl %eax,4(%edx)
.stabn 68,0,13,.LM10-g__Fv
.LM10:
	call h__Fv
.stabn 68,0,14,.LM11-g__Fv
.LM11:
	addl $-12,%esp
	leal -17(%ebp),%ebx
	pushl %ebx
	call __7CleanMe
	movl 4(%edi),%edx
	movl 4(%edx),%eax
	movl %eax,-32(%ebp)
	movl $_$_7CleanMe,-28(%ebp)
	movl %ebx,-24(%ebp)
	leal -32(%ebp),%eax
	movl %eax,4(%edx)
.stabn 68,0,15,.LM12-g__Fv
.LM12:
	addl $32,%esp
	call h__Fv
	movl 4(%edi),%edx
	movl 4(%edx),%eax
	movl (%eax),%eax
	movl %eax,4(%edx)
	addl $-8,%esp
	pushl $2
	pushl %ebx
	call _$_7CleanMe
	addl $16,%esp
	movl 4(%edi),%edx
	movl 4(%edx),%eax
	movl (%eax),%eax
	movl %eax,4(%edx)
	addl $-8,%esp
	pushl $2
	pushl %esi
	call _$_7CleanMe
	leal -56(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
