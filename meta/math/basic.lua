require("meta.math.utils")

local function inv_sqrt()
	FunctionBegin("float", "fast_inv_sqrt", "float v")
	if IS_HEADER then
		return
	end
	io.write("\tfloat vhalf = 0.5f * v;\n")
	io.write("\tint i = *(int*)&v;\n")
	io.write("\ti = 0x5f3759df - (i >> 1);\n")
	io.write("\tv = *(float*)&i;\n")
	io.write("\tv = v * (1.5f - vhalf * v * v);\n")
	io.write("\treturn v;\n")
	FunctionEnd()
end

local function helpers()
	if IS_HEADER then
		return
	end
	io.write("#define FLOAT_EPSILON 0.000001\n")
	io.write("#define min(a, b) ((a < b) ? a : b)\n")
	io.write("#define max(a, b) ((a > b) ? a : b)\n")
	io.write("#define abs(a) ((a) < 0 ? -(a) : (a))\n")
	io.write("#define float_eq(a, b) ((abs((a) - (b))) <= (FLOAT_EPSILON*max(1.0f, max(abs(a), abs(b)))))\n")
	io.write("\n")
end

function Basic()
	helpers()
	inv_sqrt()
	io.write("\n\n")
end
