local ffi = require("ffi")
local C = require("cdef")

local Widget = {}

Widget.Direction = {
	Row    = 1,
	Column = 2,
}

Widget.Color = {
	base   = C.UI_COLOR_BASE,
	yellow = C.UI_COLOR_YELLOW,
	green  = C.UI_COLOR_GREEN,
	red    = C.UI_COLOR_RED,
	blue   = C.UI_COLOR_BLUE,
	gray   = C.UI_COLOR_GRAY,
}
function Widget.Field(ui, label, value, props)
	props = props or {}
	local field_props = ffi.new("UIFieldProps", {
		position = ui.context.head,
		gap      = ui.context.field_gap,
		color    = Widget.Color[props.color or "base"],
	})
	C.ui_field(field_props, label, tostring(value))
	Widget.Gap(ui)
end

local ButtonVariant = {
	primary = C.BUTTON_PRIMARY,
	green   = C.BUTTON_GREEN,
	red     = C.BUTTON_RED,
	yellow  = C.BUTTON_YELLOW,
}
function Widget.Button(ui, label, props)
	props = props or {}
	local box = ffi.new("Rect2i", {
		x = ui.context.head.x,
		y = ui.context.head.y,
		w = props.width or 48,
		h = props.height or 18
	})
	local options = ffi.new("ButtonOptions", {
		variant        = ButtonVariant[props.variant or "primary"],
		disabled       = props.disabled or false,
		label_offset_x = props.label_offset_x or 0,
		label_offset_y = props.label_offset_y or 0,
		skip_hitheck   = not ui.context.hovering
	})
	Widget.Gap(ui)
	return C.ui_button_ex(box, label, options)
end

-- TODO: This should act more like a stack, but for now it's fine,
--       Since I don't expect complex UIs yet
function Widget.BeginRow(ui, props)
	props = props or {}
	table.insert(ui.context.stack, {
		direction = ui.context.direction,
		padding = ui.context.padding,
	})
	ui.context.direction = Widget.Direction.Row
	ui.context.padding = props.padding or 14
end

function Widget.EndRow(ui)
	local last = ui.context.stack[#ui.context.stack]
	ui.context.direction = last.direction
	ui.context.padding = last.padding
	table.remove(ui.context.stack)
end

function Widget.Gap(ui, size)
	local head = ui.context.head
	size = size or ui.context.padding or 14
	if ui.context.direction == Widget.Direction.Column then
		head.y = head.y + size
	else
		head.x = head.x + size
	end
end

return Widget
