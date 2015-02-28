#include "utils.h"

#pragma warning(disable : 4035)	// Disable no return value warning

void* CUtils::memcpy64(const void *dest, const void *src, const size_t count)
{
	// Quote of the day:  Whatcha mean this isn't cross platfom?
	// It runs on Intel, AMD and (probably in monthly installments) Cyrix,
	// What more do ya want? :D
	
	// 64 bit memory copy using MMX, optimised for count divisable by 8
	__asm
	{
			mov esi, dword ptr src	// Put src and dest in esi and edi
			mov edi, dword ptr dest
			
			mov edx, count			// Put count into edx and ebx
			mov ebx, edx
			
			prefetchnta [esi]		// Prefetch the data to be copied
			shr edx, 3				// edx = count / 8				
			mov ecx, edx			// Set up qword counter
			shl edx, 3				// edx = edx * 8
			sub ebx, edx			// ebx = remainder from count / 8
			test ebx, ebx			// Test for a remainder (hopefully there won't be one)
			jnz Copy32				// Damnit, we got a remainder :[
ALIGN 16
Copy64:								// Align loop to 16 byte boundary
			movq mm0, [esi]			// 64 bit MMX copy, now this is what I'm talking about :D
			movq [edi], mm0			// Ooh, suits you sir, ooh
			add esi, 8				// Do you like it sir, do you?
			add edi, 8				// Oooh!
			dec ecx					// :D
			jnz Copy64
			jmp End

			// Code Bloat Start
Copy32:
			cmp ebx, 4				// Is remainder less then 4?
			jb Copy8				// Yes, so jump to byte copy
			mov eax, [esi]			// No, so copy a dword, we won't bother testing for words
			mov [edi], eax			// We won't bother with movsd either, slower on P2+
			add esi, 4					
			add edi, 4
			sub ebx, 4				// Set up counter for remaining bytes
			jz End					// End if no more bytes to copy
ALIGN 16
Copy8:
			mov al, [esi]			// Standard 1 byte copy
			mov [edi], al			// Worse case scenario is 1 dword and 3 byte copies :[
			inc esi					// You always get some muppet who wants to copy 263 bytes
			inc edi					// :/
			dec ebx					// Test if we have reached end
			jnz Copy8
			test ecx, ecx			// Jump to 64 bit copy if there are any
			jnz Copy64
			// Code Bloat End
End:
			emms					// Leave MMX state (should be femms for AMD)
			mov eax, dword ptr dest	// Return pointer to dest
	}
}

#pragma warning(default : 4035)	// Reactivate no return value warning
