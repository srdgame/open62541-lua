local opcua = require 'opcua'
--[[
opcua.setLogger(function(...)
	print(...)
end)
]]--

local config = opcua.ConnectionConfig.new()
config.protocolVersion = 0
config.sendBufferSize = 65535
config.recvBufferSize = 65535
config.maxMessageSize = 0
config.maxChunkCount = 0

--local client = opcua.Client.new("opc.tcp://172.30.1.141:4840", 5000, 10 * 60 * 1000, config)
local client = opcua.Client.new("opc.tcp://127.0.0.1:4840", 5000, 10 * 60 * 1000, config)
--local client = opcua.Client.new("opc.tcp://172.30.11.121:4840", 5000, 10 * 60 * 1000, config)
local r, err = client:connect_username("user1", "password")
--local r, err = client:connect()
print(r, err)

local root = client:getRootNode()
print("Root:", root, root:getBrowseName())
print("Server:", client:getServerNode())

--[[
for _, v in ipairs(client:GetServerNamespaces()) do
	print(v)
end

]]--
local objects = client:getObjectsNode()
print(objects)
for _,v in ipairs(objects:getChildren()) do
	print(v, v:getBrowseName(), v.nodeClass)
	for _,v in ipairs(v:getChildren()) do
		print(v, v:getBrowseName())
	end
end

local id = opcua.NodeId.new(2, 99)
local id = opcua.NodeId.new(2, 1)
print( 'getNode', client:getNode(id))

local idx = client:getNamespaceIndex("http://iot.symid.com");
if not idx then
	idx = client:getNamespaceIndex("http://iot.symid.com/IDIDIDIDID")
end
print('getNamespaceIndex', idx)
print('getNamespaceIndex2', client:getNamespaceIndex("http://iot.symid.com/aaa") )

assert(idx)
local id = opcua.NodeId.new(idx, "new_object_from_client")

local attr = opcua.ObjectAttributes.new()
attr.displayName = opcua.LocalizedText.new("en_US", "Client Added Node")
attr.description = opcua.LocalizedText.new("en_US", "Description")
local obj, err = objects:addObject(id, "NewObject_Client", attr)
print('addObject', obj, err)
if not obj then
	obj, err = client:getNode(id)
	print('getNode', obj, err)
	print(client:getNode(4, "ee"))
end

local attr = opcua.VariableAttributes.new()
attr.displayName = opcua.LocalizedText.new("en_US", "Variable")
attr.description = opcua.LocalizedText.new("en_US", "Description")
attr.description = opcua.LocalizedText.new("en_US", "Description")
attr.description = opcua.LocalizedText.new("en_US", "Description")
attr.value = opcua.Variant.new(false)
attr.value = opcua.Variant.new(1)
attr.value = opcua.Variant.new(1.1)
attr.value = opcua.Variant.new("EEE")
local var, err = obj:addVariable(opcua.NodeId.new(idx, "new_variable_from_client"), "NewVariable_Client", attr)
print('addVariable', var, err)

local var, err = objects:getChild({idx..":NewObject", "MyVariable"})
if var then
	print("getChild", var, err)
	print("Var user write mask", var:getUserWriteMask(), "Var user accessl Level", var:getUserAccessLevel())
	local dv = var:getDataValue()
	print('Value of MyVariable', dv.value)
	print('Source Timestamp of MyVariable', dv.sourceTimestamp)
	print('Server Timestamp of MyVariable', dv.serverTimestamp)
	--var.dataValue = opcua.DataValue.new(opcua.Variant.new('ddddddddddddd'))
	local dv = opcua.DataValue.new(opcua.Variant.new('ddddddddddddd2'))
	dv.sourceTimestamp = opcua.DateTime.fromUnixTime(os.time())
	var:setDataValue(dv)
	local var = objects:getChild({idx..":NewObject"})
	print('NewObject', var)
	print('MyVariable', var:getChild("MyVariable"))

	local prop = var:getChild("MyProperty")
	prop:setValue(opcua.Variant.new("i am a test"))
else
	print("Newobject->MyVariable does not exits")
end

--client:deleteNode(var, true)

client:disconnect()
print('DONE')

