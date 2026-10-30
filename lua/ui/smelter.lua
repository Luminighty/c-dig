local Window = require("ui.window")

local Smelter = {}

Smelter.Id = "smelter"

Smelter.Container = {
	Type = Window,
	title = "Smelt-O-Mata",
	size = { w = 120, h = 100 },
}

function Smelter.Draw(ui, props)
	local smelter = props.smelter

	ui:Field("Status", FieldStatus(smelter))
	ui:Field("Power", FieldPower(smelter))

	ui:Gap()

	ui:BeginRow({ padding = 52 })

	local start_disabled = smelter.enabled or smelter.power <= 0
	if ui:Button("Start", {variant = "green", disabled = start_disabled}) then
		smelter.enabled = true
	end

	local stop_disabled = not smelter.enabled
	if ui:Button("Stop", {variant = "red", disabled = stop_disabled}) then
		smelter.enabled = false
	end

	ui:EndRow()
end

function FieldStatus(smelter)
	if smelter.power == 0 then
		return "No Power", { color = "gray" }
	elseif smelter.enabled then
		return "Online", { color = "green" }
	else
		return "Offline", { color = "red" }
	end
end

function FieldPower(smelter)
	local color = "blue"
	if smelter.power < 100 then color = "green" end
	if smelter.power < 50 then color = "yellow" end
	if smelter.power < 20 then color = "red" end
	return string.format("%d %%", smelter.power), { color = color }
end

return Smelter
