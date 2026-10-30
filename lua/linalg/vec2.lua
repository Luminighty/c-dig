local ffi = require("ffi")

---@class Vec2
---@field x number
---@field y number

local Vec2
local mt = {
	__add = function (a, b) return Vec2(a.x + b.x, a.y + b.y) end,
	__sub = function (a, b) return Vec2(a.x - b.x, a.y - b.y) end,
	__tostring = function (v) return "vec2(" .. v.x .. "," .. v.y .. ")" end,
	__index = {
		lengthsqr = function (a) return (a.x * a.x) + (a.y * a.y) end,
		normalize = function (a)
			local inv_len = ffi.C.fast_inv_sqrt(a:lengthsqr())
			return Vec2(
				a.x * inv_len,
				a.y * inv_len
			)
		end,
		scale = function (a, scale)
			return Vec2(
				a.x * scale,
				a.y * scale
			)
		end,
		move_towards = function (from, target, max_distance)
			local delta = target - from
			local dist_sqr = max_distance * max_distance
			if delta:lengthsqr() > dist_sqr then
				local move = delta:normalize():scale(max_distance)
				return from + move
			else
				return target
			end
		end
	}
}

---@alias Vec2Ptr `Vec2*`

---@type function(x: number, y: number): Vec2
Vec2 = ffi.metatype("Vec2", mt)

return Vec2
