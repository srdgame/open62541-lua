local opcua = require 'opcua'

local config = opcua.ConnectionConfig.new()
config.protocolVersion = 0
config.sendBufferSize = 65535
config.recvBufferSize = 65535
config.maxMessageSize = 0
config.maxChunkCount = 0

local client = opcua.Client.new(5000, 10 * 60 * 1000, config)
client:setLogger(function(...)
	print('------------------------')
	print(...)
end)
--local r, err = client:connect_username("opc.tcp://127.0.0.1:4840", "user1", "password")
local r, err = client:connect("opc.tcp://127.0.0.1:4840")
print(r, err)

local root = client:getRootNode()
print("Root:", root, root.browse_name)


--[[
for _, v in ipairs(client:GetServerNamespaces()) do
	print(v)
end

]]--
local objects = client:getObjectsNode()
for _,v in ipairs(objects:getChildren()) do
	print(v, v.browse_name)
	for _,v in ipairs(v:getChildren()) do
		print(v, v.browse_name)
	end
end

local idx = client:getNamespaceIndex("http://iot.symid.com");
print('getNamespaceIndex', idx)
print('getNamespaceIndex2', client:getNamespaceIndex("http://iot.symid.com/aaa") )

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

--client:deleteNode(var, true)

client:disconnect()
print('DONE')

