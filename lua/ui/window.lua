require("table.clear")
local Widget = require("ui.widget")
local C = require("cdef")
local ffi = require("ffi")

local Window = {}
Window.mt = {}


function Window:Context(props)
	return {
		field_gap = props.field_gap or 48,
		padding = props.padding or 14,
		head = { x = 0, y = 0 },
		direction = Widget.Direction.Column,
		hovering = false,
		stack = {}
	}
end

function Window:New(window_props, on_draw, props)
	props = props or {}

	local window = {}
	window.id = window_props.id
	window.context = Window:Context(window_props)
	window.state = ffi.new("Window", {
		title = window_props.title,
		box = window_props.box,
	})
	window.on_draw = on_draw
	window.props = props

	setmetatable(window, Window.mt)
	return window
end

local MARGIN = {
	top = 22,
	left = 8,
	bottom = 8,
	right = 8,
}
MARGIN.horizontal = MARGIN.left + MARGIN.right
MARGIN.vertical = MARGIN.top + MARGIN.bottom
function Window:Draw(window, hovering)
	local result = C.ui_window(window.state)
	if result == C.WINDOW_EVENT_CLOSE then
		print("CLOSE")
		return false
	end
	window.context.hovering = hovering
	local box = window.state.box
	window.context.head.x = box.x + MARGIN.left
	window.context.head.y = box.y + MARGIN.top
	---@diagnostic disable-next-line: undefined-field
	table.clear(window.context.stack)
	window:on_draw(window.props)

	local head = window.context.head
	box.w = math.max(box.w, head.x - box.x + MARGIN.right + MARGIN.left)
	box.h = math.max(box.h, head.y - box.y + MARGIN.vertical)

	return true
end

function Window:IsHovering(window, mouse)
	return mouse.x >= window.state.box.x and
	       mouse.y >= window.state.box.y and
	       mouse.x <= window.state.box.x + window.state.box.w and
	       mouse.y <= window.state.box.y + window.state.box.h
end

Window.mt.__index = {}
for key, value in pairs(Widget) do
	Window.mt.__index[key] = value
end


return Window
