/*
 * Assembly-language implementations of the drawing functions
 */

/* Offsets of struct Image fields */
#define IMAGE_WIDTH_OFFSET   0
#define IMAGE_HEIGHT_OFFSET  4
#define IMAGE_DATA_OFFSET    8

/* Offsets of struct Rect fields */
#define RECT_X_OFFSET        0
#define RECT_Y_OFFSET        4
#define RECT_WIDTH_OFFSET    8
#define RECT_HEIGHT_OFFSET   12

	.section .text

/***********************************************************************
   Helper functions
   You should declare these as .globl so that
   they can be called from the unit test code.
 ***********************************************************************/

/* TODO: implement your helper functions here */

/*
 * Return whether the point is within the range of the image
 *
 * Parameters:
 * 	%rdi - a pointer to the image
 * 	%esi - the x coordinate of the point
 *	%edx - the y coordinate of the point
 *
 * Return: (in %rax)
 *	A int32_t that is 0 if the point is not in the range and 1 if otherwise 
 */

	.globl in_bounds
in_bounds:
	/*save three register for future use and also align the stack frame*/
	pushq %r12
	pushq %r13
	pushq %r14

	/*save input in these three registers*/
	movq %rdi, %r12
	movl %esi, %r13d
	movl %edx, %r14d

	/*test if the x coordinate is within the range*/
	movl %r13d, %edi	/*move the x coordinate into rdi*/
	movl $0, %esi	/*move the 0 to %rsi as the min*/
	movl IMAGE_WIDTH_OFFSET(%r12), %edx	/*move the width into rdx as the max*/
	call clamp
	movl %eax, %r10d /*save the result in %r10*/

	/*test if the y coordinate is within the range*/
	movl %r14d, %edi	/*move the y coordinate into rdi*/
	movl $0, %esi	/*move the 0 to %rsi as the min*/
	movl IMAGE_HEIGHT_OFFSET(%r12), %edx	/*move the height into rdx as the max*/
	call clamp
	addl %eax, %r10d /*add the result to %r10*/

	/*compare the result, %r10 should contains 2 if both conditions are met and less than 2 if otherwise*/
	movl $2, %r11d
	cmpl %r10d, %r11d
	movl $1, %eax	/*restore the rax*/
	ja .Lout

	/*retore the value in the three register and retun the value*/
	popq %r14
	popq %r13
	popq %r12
	ret
	
.Lout:
	movq $0, %rax
	popq %r14
	popq %r13
	popq %r12
	ret
	

	.globl compute_index
compute_index:
	/* 
	   %rdi   pointer to a struct Image
	   %esi   x coordinate
	   %edx   y coordinate
	*/

	movl %edx, %eax   //put the value of y into %eax
	movl IMAGE_WIDTH_OFFSET(%rdi), %r10d //put the value of image width into r10d
	imull %r10d, %eax
	addl %esi, %eax
	ret

/*
 * Return whether the value is min <= value < max
 *
 * Parameters:
 * 	%rdi - input value for comparision
 * 	%rsi - min
 *	%rdx - max
 *
 * Return: (in %rax)
 *	A int32_t that is 0 if the value is not in the range and 1 if otherwise 
 */
	.globl clamp
clamp:
	/*Set the default value of %rax as true*/
	movl $1, %eax

	/*Compare the value with the min*/
	cmpl %esi, %edi
	jl .Ltest

	/*Compare the value with the max*/
	cmpl %edi, %edx
	jle .Ltest

	ret

.Ltest:
	movl $0, %eax
	ret

	.globl get_r
get_r:

	/* %rdi    value of color to compute */
	shrl $24, %edi    //right shift color value by 24 bits
	movl %edi, %eax    //mov value of rdi to rax (prepare for return)
	ret

	.globl get_g
get_g:

	/* %rdi    value of color to compute */
	shll $8, %edi   //left shift color value by 8
	shrl $24, %edi  //right shift by 24
	movl %edi, %eax //prepare for return
	ret

	.globl get_b
get_b:
	/* %rdi    value of color to compute */
	shll $16, %edi   //left shift color value by 16
	shrl $24, %edi  //right shift by 24
	movl %edi, %eax //prepare for return
	ret

	.globl get_a
get_a:
	shll $24, %edi   //left shift color value by 24
	shrl $24, %edi  //right shift by 24
	movl %edi, %eax //prepare for return
	ret
	ret

/* 
 * %rdi     value of fg (frontground color, alpha value of which is used)
 * %rsi		value of bg (background color)
 * %rdx     value of alpha 

   %r12 temporary value of 255-alpha
   %r13 value 255 for division

 */
	.globl blend_components
blend_components:
	pushq %r12         /* push %r12 onto stack */
	pushq %r13         /* push %r13 onto stack */
	imulq %rdx, %rdi   /* compute alpha * fg, put the value in %rdi */
	movq $255, %r12    /* put 255 value in %r12 */
	subq %rdx, %r12    /* compute 255 - alpha, put the value in %r12 */
	imulq %r12, %rsi   /* compute (255-alpha) * bg, put the value in %rsi */
	movq %rdi, %rax    /* put alpha * fg in rax, prepare for division */
	addq %rsi, %rax    /* add alpha * gf and 255-alpha * bg, save to %rax */
	movq $0, %rdx      /* set rdx to 0 */
	movq $255, %r13
	divq %r13         /* divide by 255 */
	popq %r13
	popq %r12
	ret

/* 
 * %edi     value of fg (frontground color, alpha value of which is used)
 * %esi		value of bg (background color)
 * %e12     value of fg store locally
 * %e13     value of bg store locally
 * %e14     value of get_somecolor(fg) each time
 * %e15     value of get_somecolor(bg) each time
 * -4(%rbp) value of alpha

 */

	.globl blend_colors
blend_colors:
	pushq %r12   /*push %r12 onto the stack */
	pushq %r13   /*push %r13 onto the stack */
	pushq %r14
	pushq %r15
	pushq %rbp   /*push %rbp onto the stack */
	movq %rsp, %rbp  /*store the value of rsp at rbp (start of stack frame) */
	subq $16, %rsp  /*reserve 16 memory space in stack frame */
	movl %edi, %r12d  /*store value of fg in r12 */
	movl %esi, %r13d  /*store value of bg in r13 */
	movl %r12d, %edi  /*set argument */
	call get_a   /*%rdi remain unchanged, the function only have one argument now */
	movl %eax, -4(%rbp)  /*put the value of alpha at rbp-4 */

	movl %r12d, %edi /*start call blend_components first time */
	call get_r
	movl %eax, %r14d /*put value of get_r(fg) into %r14 */
	movl %r13d, %edi /*put the value of bg in to rdi(first argument) */
	call get_r
	movl %eax, %r15d /*put value of get_r(bg) into %r15 */
	movl %r14d, %edi /*set first argument of blend_components */
	movl %r15d, %esi /*set second argument */
	movl -4(%rbp), %edx /*set third argument */
	call blend_components
	movl %eax, -8(%rbp) /*store blended red color at rbp-8 */ 

	movl %r12d, %edi /*start call blend_components */
	call get_g
	movl %eax, %r14d /*put value of get_r(fg) into %r14 */
	movl %r13d, %edi /*put the value of bg in to rdi(first argument) */
	call get_g
	movl %eax, %r15d /*put value of get_r(bg) into %r15 */
	movl %r14d, %edi /*set first argument of blend_components */
	movl %r15d, %esi /*set second argument */
	movl -4(%rbp), %edx /*set third argument */
	call blend_components
	movl %eax, -12(%rbp) /*store blended red color at rbp-12 */

	movl %r12d, %edi /*start call blend_components */
	call get_b
	movl %eax, %r14d /*put value of get_r(fg) into %r14 */
	movl %r13d, %edi /*put the value of bg in to rdi(first argument) */
	call get_b
	movl %eax, %r15d /*put value of get_r(bg) into %r15 */
	movl %r14d, %edi /*set first argument of blend_components */
	movl %r15d, %esi /*set second argument */
	movl -4(%rbp), %edx /*set third argument */
	call blend_components
	movl %eax, -16(%rbp) /*store blended red color at rbp-16 */


	movl -8(%rbp), %edi  /*move stored red color to edi */
	shll $24, %edi  /*left shift 24 */
	movl -12(%rbp), %esi  /*move stored green cikir to esi */
	shll $16, %esi /*left shift 16 */
	movl -16(%rbp), %edx /*move stored blue color to edx */
	shll $8, %edx /*left shift 8 */
	movl $255, %eax /*put 255 */
	addl %edi, %eax
	addl %esi, %eax
	addl %edx, %eax

	addq $16, %rsp
	popq %rbp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	ret

	.globl set_pixel
set_pixel:
	/*
	  %rdi    pointer to a image
	  %esi    index to change
	  %edx    color to put in
	  %r12    index value
	  %r13    address of image->data
	*/


	pushq %r12
	pushq %r13
	subq $8, %rsp
	movl %esi, %r12d   /*store the index to %r12 */
	movq IMAGE_DATA_OFFSET(%rdi), %r13  /*put address of image->data to r13 */
	movl (%r13, %r12, 4), %esi  /*put the current color value in image to %esi (second argument) */
	movl %edx, %edi   /*set the first argument */
	call blend_colors
	movl %eax, (%r13, %r12, 4)   /*move the blended color to the memory position */

	addq $8, %rsp
	popq %r13
	popq %r12
	ret

/* 
 * Return the square of a uint64_t
 *
 * Parameters:
 * 	%rdi - the value to square
 *
 * Return: (in %rax)
 *	the square of the uint64_t value
 */

	.globl square
square:
	/*sqare the value in %rdi*/
	imulq %rdi, %rdi
	/*move the value to rax for return*/
	movq %rdi, %rax
	/*return the value*/
	ret

	.globl square_dist

/* 
 * Return the distance of two point on the xy plane
 *
 * Parameters:
 * 	%rdi - x1
 *	%rsi - y1
 *  %rdx - x2
 * 	%rcx - y2
 *
 * Return: (in %rax)
 *	the sum of geometric distance between (x1,y1) and (x2, y2)
 */

square_dist:
	/*Align the stack*/
	subq $8, %rsp

	/*calculate the square of (x1-x2) and store the result in %r10*/
	subq %rdx, %rdi
	call square
	movq %rax, %r10

	/*calculate the square of (y1-y2)*/
	subq %rsi, %rcx
	movq %rcx, %rdi
	call square

	/*add the result into rax for return and restore the stack*/
	addq %r10, %rax
	addq $8, %rsp
	ret

/***********************************************************************
   Public API functions
 ***********************************************************************/

/*
 * Draw a pixel.
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %esi     - x coordinate (pixel column)
 *   %edx     - y coordinate (pixel row)
 *   %ecx     - uint32_t color value 
 * Register usage:
 *   %r12     - pointer of image
 *   %r13     - x-coordinate
 *   %r14     - y-coordinate
 *   %r15     - color value to be put
 */
	.globl draw_pixel
draw_pixel:
	/* TODO: implement */
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	subq $8, %rsp
	movq %rdi, %r12   /*put the pointer to r12 */
	movl %esi, %r13d  /*put x coordinate */
	movl %edx, %r14d  /*put y coordinate */
	movl %ecx, %r15d  /*put color */
	call in_bounds
	cmpl $0, %eax
	je .Lreturn   /*jump is ! inbounds */
	movq %r12, %rdi 
	movl %r13d, %esi
	movl %r14d, %edx  /*set up argument to compute the index */
	call compute_index 
	movl %eax, %esi  /*index is the second argument */
	movq %r12, %rdi
	movl %r15d, %edx
	call set_pixel 

.Lreturn:
	addq $8, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	ret


/*
 * Draw a rectangle.
 * The rectangle has rect->x,rect->y as its upper left corner,
 * is rect->width pixels wide, and rect->height pixels high.
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %rsi     - pointer to struct Rect
 *   %edx     - uint32_t color value
 */
	.globl draw_rect
draw_rect:
	/*save these registers for future use and also align the stack frame*/
	pushq %rbx
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	subq $8, %rsp
	/*save input in these registers*/
	movq %rdi, %r14
	movq %rsi, %r12
	movl %edx, %r13d
	/*These %ebp and %ebx will be used for keep track of changing of x and y coordinate*/
	movl $0, %ebx
	movl $0, %ebp

	/*The outer loop*/
	jmp .Louter

.Louter:
	movl RECT_HEIGHT_OFFSET(%r12), %r15d
	cmp %ebx, %r15d
	/*Before entering the inner loop again, we need to retsore the value in %r11d*/
	movl $0, %ebp
	/*if the current height is less than the upper bound, we jump to the inner loop*/
	ja .Linner
	/*if the loop ends, jump to the .Lout and reset all the callee register*/
	jmp .LEnd

.Linner:
	/*The inner loop*/
	movl RECT_WIDTH_OFFSET(%r12), %r15d
	cmp %ebp, %r15d
	/*if the current width is less than the upper bound, we jump to the inner loop*/
	ja .Lbody
	/*If we get into the loop, we need to increase the value of $ebx by 1*/
	add $1, %ebx
	jmp .Louter

.Lbody:
	/*%rdi will stay the same while %rsi, %rdx, and %rcx will be updated*/
	movq %r14, %rdi
	movl RECT_X_OFFSET(%r12), %esi
	movl RECT_Y_OFFSET(%r12), %edx
	add %ebp, %esi
	add %ebx, %edx
	movl %r13d, %ecx
	call draw_pixel	/*call function, with no return value*/
	/*If we get into the loop, we need to increase the value of $ebp by 1*/
	add $1, %ebp
	/*As the loop end, We jump back to its upper loop*/
	jmp .Linner

.LEnd:
	add $8, %rsp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	popq %rbx
	ret

/*
 * Draw a circle.
 * The circle has x,y as its center and has r as its radius.
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %esi     - x coordinate of circle's center
 *   %edx     - y coordinate of circle's center
 *   %ecx     - radius of circle
 *   %r8d     - uint32_t color value
 */
	.globl draw_circle
draw_circle:
	/*save these registers for future use and also align the stack frame*/
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	/*saving all the input on the stack for future use*/
	pushq %rbp  /*put %rbp onto stack*/
	movq %rsp, %rbp  
	subq $24, %rsp  /*allocate stack space*/
	movq %rdi, -8(%rbp) /*pointer to struct Image*/
	movl %esi, -12(%rbp) /*x coordinate of circle's center*/
	movl %edx, -16(%rbp) /*y coordinate of circle's center*/
	movl %ecx, -20(%rbp) /*radius of circle*/
	movl %r8d, -24(%rbp) /*uint32_t color value*/

	/*Set the variable of looping to variable*/
	movl $0, %r12d /*a*/
	movl $0, %r13d /*b*/
	movl IMAGE_WIDTH_OFFSET(%rdi), %r14d /*Width*/
	movl IMAGE_HEIGHT_OFFSET(%rdi), %r15d /*Height*/
	/*Start Looping*/
	jmp .Lfirst

.Lfirst:
	/*Outer Loop*/
	cmp %r12d, %r14d
	/*When ever we enter the first loop, we must set b to 0*/
	movl $0, %r13d
	/*If a is less than width we get into the loop*/
	ja .Lsecond
	jmp .Lstop

.Lsecond:
	/*Inner Loop*/
	cmp %r13d, %r15d
	/*If b is less than Height we get into the loop*/
	ja .Lmiddle
	add $1, %r12d
	jmp .Lfirst
	
.Lmiddle:
	/*Set the value of the distance function*/
	movl -12(%rbp), %edi /*x*/
	movl -16(%rbp), %esi /*y*/
	movl %r12d, %edx/*a*/
	movl %r13d, %ecx/*b*/
	call square_dist /*return the distance and store it in %rax*/
	movq %rax, %rbx
	
	/*Set the value of the square function*/
	movl -20(%rbp), %edi
	call square

	/*compare the distance, if within the range call the draw_pixel function*/
	cmp %rbx, %rax
	jae .Lcheck
	jmp .Ldefault

.Lcheck:
	/*Set the function for draw pixel*/
	movq -8(%rbp), %rdi /*Image Pointer*/
	movl %r12d, %esi /*a*/
	movl %r13d, %edx /*b*/
	movl -24(%rbp), %ecx /*color*/
	call draw_pixel
	jmp .Ldefault

.Ldefault:
	add $1, %r13d
	jmp .Lsecond

.Lstop:
	/*Return value to callee saved register and align the stack*/
	addq $24, %rsp
	popq %rbp
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	ret

/*
 * Draw a tile by copying all pixels in the region
 * enclosed by the tile parameter in the tilemap image
 * to the specified x/y coordinates of the destination image.
 * No blending of the tile pixel colors with the background
 * colors should be done.
 *
 * Parameters:
 *   %rdi - pointer to Image (dest image)
 *   %esi - x coordinate of location where tile should be copied
 *   %edx - y coordinate of location where tile should be copied
 *   %rcx - pointer to Image (the tilemap)
 *   %r8  - pointer to Rect (the tile)
 */
 	/*
	%r12   value of index for first loop
	%r13   value of index for second loop
	-8(%rbp) pointer of dest image
	-12(%rbp) x coordinate
	-16(%rbp) y coordinate
	-24(%rbp) pointer to spritemap
	-32(%rbp) pointer to sprite
	-36(%rbp) img->width
	-40(%rbp) img->height
	-44(%rbp) spritemap->width
	-48(%rbp) spritemap->height
	-52(%rbp) sprite->x
	-56(%rbp) sprite->y
	-60(%rbp) sprite->width
	-64(%rbp) sprite->height
	*/
	.globl draw_tile
draw_tile:
	/* TODO: implement */

	pushq %rbp  /*put %rbp onto stack */
	movq %rsp, %rbp  
	subq $64, %rsp  /*allocate stack space */
	pushq %r12
	pushq %r13
	pushq %r14
	subq $8, %rsp
	movq %rdi, -8(%rbp)  /*store img */
	movl %esi, -12(%rbp) /*store x */
	movl %edx, -16(%rbp) /*store y1 */
	movq %rcx, -24(%rbp) /*store spritemap */
	movq %r8, -32(%rbp)  /*store sprite */
	movl IMAGE_WIDTH_OFFSET(%rdi), %eax 
	movl %eax, -36(%rbp) /*store img->width */
	movl IMAGE_HEIGHT_OFFSET(%rdi), %eax
	movl %eax, -40(%rbp) /*store img->height */
	movl IMAGE_WIDTH_OFFSET(%rcx), %eax
	movl %eax,-44(%rbp) /*store spritemap->width */
	movl IMAGE_HEIGHT_OFFSET(%rcx), %eax
	movl %eax,-48(%rbp) /*store spritemap->height */
	movl RECT_X_OFFSET(%r8), %eax
	movl %eax,-52(%rbp) /*store sprite->x */
	movl RECT_Y_OFFSET(%r8), %eax
	movl %eax,-56(%rbp) /*store sprite->y */
	movl RECT_WIDTH_OFFSET(%r8), %eax
	movl %eax,-60(%rbp) /*store sprite->width */
	movl RECT_HEIGHT_OFFSET(%r8), %eax
	movl %eax,-64(%rbp) /*store sprite->height */

	/*value storage finished */

	movq -24(%rbp), %rdi  /*set first argument spritemap */
	movl -52(%rbp), %esi  /*set second argument, sprite->x */
	movl -56(%rbp), %edx /*set third argument, sprite->y */
	call in_bounds
	cmpl $0, %eax  /*compare result with 0 */
	je .TreturnTile

	movq -24(%rbp), %rdi  /* set first argument spritemap */
	movl -52(%rbp), %esi  /*set second argument, sprite->x */
	movl -56(%rbp), %edx /*set third argument, sprite->y */
	movl -60(%rbp), %r10d /*store sprite->width in r10d */
	movl -64(%rbp), %r11d /*store sprite->height in r11d */
	addl %r10d, %esi /*sprite->x + width */
	addl %r11d, %edx /*sprite->y + height */
	decl %esi
	decl %edx
	call in_bounds
	cmpl $0, %eax
	je .TreturnTile

	movl $0, %r12d  /*store the index for first loop in %r12d*/
	

.TFirstLoop:
	movl -60(%rbp), %r10d  /*put sprite->width into r10d */
	cmpl %r12d, %r10d  /*if i == sprite->width, terminate loop and function return */
	je .TreturnTile
	movl $0, %r13d  /*set j = 0 */

.TSecondLoop:
	movl -64(%rbp), %r11d  /*put sprite->height into r11d */
	cmpl %r11d, %r13d  /*if j == sprite->height, terminate second loop and jump to first loop */
	je .TSecondLoopTerminate


	movq -8(%rbp), %rdi  /*set first argument img */
	movl -12(%rbp), %esi  /*set second argument */
	addl %r12d, %esi  /*x + i */
	movl -16(%rbp), %edx /*set third argument */
 	addl %r13d, %edx   /*y + j */
	call in_bounds
	cmpl $0, %eax  /*compare result with 0 */
	je .TOutOfBound

	movq -24(%rbp), %rdi /*set first argument spritemap */
	movl -52(%rbp), %esi  /*set second argument */
	addl %r12d, %esi  /*sprite->x + i */
	movl -56(%rbp), %edx /*set third argument */
	addl %r13d, %edx   /*sprite->y + j */
	call compute_index
	movl %eax, %r14d /*store index to r14 */
	movq -8(%rbp), %rdi  /*set first argument img */
	movl -12(%rbp), %esi  /*set second argument */
	addl %r12d, %esi  /*x + i */
	movl -16(%rbp), %edx /*set third argument */
	addl %r13d, %edx   /*y + j */
	call compute_index  
	movl %eax, %r11d  /*get index for image in r11 */
	movq -24(%rbp), %r9  /*store spritemap in r9 */
	movq IMAGE_DATA_OFFSET(%r9), %rdi  /*store spritemap->data address in rdi */
	movl (%rdi, %r14, 4), %ecx /*get sprite->map[index] */
	movq -8(%rbp), %r9  /*store img in r9 */
	movq IMAGE_DATA_OFFSET(%r9), %rdi  /*stor image->data address */
	movl %ecx, (%rdi, %r11, 4) /*make the index equavilent */
	addl $1, %r13d  /*j++ */
	jmp .TSecondLoop

.TOutOfBound:
	addl $1, %r13d  /*j++ */
	jmp .TSecondLoop

.TSecondLoopTerminate:
	addl $1, %r12d  /*increment i by 1 */
	jmp .TFirstLoop

.TreturnTile:		/*point where the function return */
	addq $8, %rsp
	popq %r14
	popq %r13
	popq %r12
	addq $64, %rsp
	popq %rbp
	ret

/*
 * Draw a sprite by copying all pixels in the region
 * enclosed by the sprite parameter in the spritemap image
 * to the specified x/y coordinates of the destination image.
 * The alpha values of the sprite pixels should be used to
 * blend the sprite pixel colors with the background
 * pixel colors.
 *
 * Parameters:
 *   %rdi - pointer to Image (dest image)
 *   %esi - x coordinate of location where sprite should be copied
 *   %edx - y coordinate of location where sprite should be copied
 *   %rcx - pointer to Image (the spritemap)
 *   %r8  - pointer to Rect (the sprite)	
 *  
 * Memory Notation:
 *	 %r12   value of index for first loop
 *	 %r13   value of index for second loop
 *	 -8(%rbp) pointer of dest image
 *	 -12(%rbp) x coordinate
 *	 -16(%rbp) y coordinate
 *	 -24(%rbp) pointer to spritemap
 *	 -32(%rbp) pointer to sprite
 *	 -36(%rbp) img->width
 *	 -40(%rbp) img->height
 *	 -44(%rbp) spritemap->width
 * 	 -48(%rbp) spritemap->height
 *	 -52(%rbp) sprite->x
 *	 -56(%rbp) sprite->y
 *	 -60(%rbp) sprite->width
 *	 -64(%rbp) sprite->height
 */

	.globl draw_sprite
draw_sprite:
	/* TODO: implement */
	
	pushq %rbp  /*put %rbp onto stack*/
	movq %rsp, %rbp  
	subq $64, %rsp  /*allocate stack space*/
	pushq %r12
	pushq %r13
	movq %rdi, -8(%rbp)  /*store img*/
	movl %esi, -12(%rbp) /*store x*/
	movl %edx, -16(%rbp) /*store y1*/
	movq %rcx, -24(%rbp) /*store spritemap*/
	movq %r8, -32(%rbp)  /*store sprite*/
	movl IMAGE_WIDTH_OFFSET(%rdi), %eax
	movl %eax, -36(%rbp) /*store img->width*/
	movl IMAGE_HEIGHT_OFFSET(%rdi), %eax
	movl %eax, -40(%rbp) /*store img->height*/
	movl IMAGE_WIDTH_OFFSET(%rcx), %eax
	movl %eax,-44(%rbp) /*store spritemap->width*/
	movl IMAGE_HEIGHT_OFFSET(%rcx), %eax
	movl %eax,-48(%rbp) /*store spritemap->height*/
	movl RECT_X_OFFSET(%r8), %eax
	movl %eax,-52(%rbp) /*store sprite->x*/
	movl RECT_Y_OFFSET(%r8), %eax
	movl %eax,-56(%rbp) /*store sprite->y*/
	movl RECT_WIDTH_OFFSET(%r8), %eax
	movl %eax,-60(%rbp) /*store sprite->width*/
	movl RECT_HEIGHT_OFFSET(%r8), %eax
	movl %eax,-64(%rbp) /*store sprite->height*/

	/*value storage finished */

	movq -24(%rbp), %rdi  /*set first argument spritemap */
	movl -52(%rbp), %esi  /*set second argument, sprite->x */
	movl -56(%rbp), %edx /*set third argument, sprite->y */
	call in_bounds
	cmpl $0, %eax  /*compare result with 0 */
	je .LreturnSprite

	movq -24(%rbp), %rdi  /* set first argument spritemap */
	movl -52(%rbp), %esi  /*set second argument, sprite->x */
	movl -56(%rbp), %edx /*set third argument, sprite->y */
	movl -60(%rbp), %r10d /*store sprite->width in r10d */
	movl -64(%rbp), %r11d /*store sprite->height in r11d */
	addl %r10d, %esi /*sprite->x + width */
	addl %r11d, %edx /*sprite->y + height */
	decl %esi
	decl %edx
	call in_bounds
	cmpl $0, %eax
	je .LreturnSprite

	movl $0, %r12d  /*store the index for first loop in %r12d*/
	

.LFirstLoop:
	movl -60(%rbp), %r10d  /*put sprite->width into r10d*/
	cmpl %r12d, %r10d  /*if i == sprite->width, terminate loop and function return*/
	je .LreturnSprite
	movl $0, %r13d  /*set j = 0*/

.LSecondLoop:
	movl -64(%rbp), %r11d  /*put sprite->height into r11d*/
	cmpl %r11d, %r13d  /*if j == sprite->height, terminate second loop and jump to first loop*/
	je .LSecondLoopTerminate

	movq -8(%rbp), %rdi  /*set first argument img*/
	movl -12(%rbp), %esi  /*set second argument*/
	addl %r12d, %esi  /*x + i*/
	movl -16(%rbp), %edx /*set third argument*/
	addl %r13d, %edx   /*y + j*/
	call in_bounds
	cmpl $0, %eax  /*compare result with 0*/
	je .LOutOfBound

	movq -24(%rbp), %rdi /*set first argument spritemap*/
	movl -52(%rbp), %esi  /*set second argument*/
	addl %r12d, %esi  /*sprite->x + i*/
	movl -56(%rbp), %edx /*set third argument*/
	addl %r13d, %edx   /*sprite->y + j*/
	call compute_index
	movl %eax, %r10d /*store index*/
	movq -8(%rbp), %rdi  /*set first argument img*/
	movl -12(%rbp), %esi  /*set second argument*/
	addl %r12d, %esi  /*x + i*/
	movl -16(%rbp), %edx /*set third argument*/
	addl %r13d, %edx   /*y + j*/
	movq -24(%rbp), %r9  /*store spritemap in r9*/
	movq IMAGE_DATA_OFFSET(%r9), %r11  //store spritemap->data address in r11*/
	movl (%r11, %r10, 4), %ecx //get sprite->map[index], set forth argument*/
	call draw_pixel
	addl $1, %r13d  /*j++*/
	jmp .LSecondLoop

.LOutOfBound:
	addl $1, %r13d  /*j++*/
	jmp .LSecondLoop

.LSecondLoopTerminate:
	addl $1, %r12d  /*increment i by 1*/
	jmp .LFirstLoop

.LreturnSprite:		/*point where the function return*/
	popq %r13
	popq %r12
	addq $64, %rsp
	popq %rbp
	ret

/*
vim:ft=gas:
*/
