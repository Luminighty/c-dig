local UI = {}

local Components = {}
local containers = {}
local draw_order = {}

function UI.Register(Component)
	Components[Component.Id] = Component
end

function UI.Open(ComponentId, id, props)
	local container = containers[id]
	if container ~= nil then
		container.props = props
		return container
	end
	props = props or {}
	local Component = Components[ComponentId]
	local on_draw = Component.Draw
	local Container = Component.Container

	local element = Container.Type:New(Container, on_draw, props)
	containers[id] = {
		Container = Container.Type,
		element = element
	}
	draw_order[#draw_order + 1] = id
	return containers[id]
end


function UI.Close(id)
	containers[id] = nil
	for i = #draw_order,1,-1 do
		if draw_order[i] == id then
			table.remove(draw_order, i)
			break
		end
	end
end

function UI.Draw(mouse)
	local hovering_id = nil
	for i = #draw_order,1,-1 do
		local id = draw_order[i]
		local container = containers[id]
		if container.Container:IsHovering(container.element, mouse) then
			hovering_id = id
			break
		end
	end
	for _, id in ipairs(draw_order) do
		local container = containers[id]
		local hovering = hovering_id == id
		local stay_open = container.Container:Draw(container.element, hovering)
		if not stay_open then
			UI.Close(id)
		end
	end
end

return UI
