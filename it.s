.syntax unified
_start:
	.global _start
	cmp r0, #0
	itete eq
	addeq r0, #1
	addne r0, #1
	moveq r0, #1
	movne r0, #1

.data
