#ifndef MATH_H
#define MATH_H


typedef struct {
	int x;
	int y;
} Vec2;


#define vec2_apply(left, op, right) (left).x op (right.x); (left).y op (right.y);

#define vec2_op(left, op, right) ((Vec2){.x = (left).x op (right).x, .y = (left).y op (right).y})
#define vec2_add(left, right) vec2_op(left, + , right)
#define vec2_sub(left, right) vec2_op(left, - , right)
#define vec2_is_zero(vec) ((vec).x == 0 && (vec).y == 0)

#endif // MATH_H
