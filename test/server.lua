---------------
-- Server example

local opcua = require 'opcua'

--[[
opcua.setLogger(function(...)
	print(...)
end)
]]--

local server = opcua.Server.new()
local config = server.config
--server:SetEndpoint("opc.tcp://127.0.0.1:4840/freeopcua/server/")
config:setServerURI("urn:://exampleserver.freeopcua.github.io")

--print(pcall(server.GetNamespaceIndex, server, "http://examples.freeopcua.github.io"))
--local idx = server:RegisterNamespace("http://examples.freeopcua.github.io")
local idx = server:addNamespace("http://iot.symid.com")
print(idx)
--print(server:GetNamespaceIndex("http://examples.freeopcua.github.io"))
--print(idx)
local objects = server:getObjectsNode()
local nid = opcua.NodeId.new(idx, 99)

local attr = opcua.ObjectAttributes.new()
attr.displayName = opcua.LocalizedText.new("en_US", "New Object DisplayName")
attr.description = opcua.LocalizedText.new("en_US", "New Object Description")
local newobject = objects:addObject(nid, "NewObject", attr)
print(newobject, pcall(objects.addObject, objects, nid, "NewObject", attr))
--print(objects:GetChild("99:NewObject"))

local attr = opcua.VariableAttributes.new()
attr.displayName = opcua.LocalizedText.new("en_US", "My Variable DisplayName")
attr.description = opcua.LocalizedText.new("en_US", "My Variable Description")
attr.value = opcua.Variant.int32(8)
--[[
attr.writeMask = opcua.WriteMask.ALL
attr.userWriteMask = opcua.WriteMask.ALL
]]
attr.accessLevel = opcua.AccessLevel.RW
--attr.userAccessLevel = opcua.AccessLevel.READ ~ opcua.AccessLevel.READ
local myvar = newobject:addVariable(opcua.NodeId.new(idx, 98), "MyVariable", attr)

local attr = opcua.VariableAttributes.new()
attr.displayName = opcua.LocalizedText.new("en_US", "My Time Ticker")
attr.description = opcua.LocalizedText.new("en_US", "My Time Ticker Description")
attr.value = opcua.Variant.datetime(opcua.DateTime.now())
local mytick = newobject:addVariable(opcua.NodeId.new(idx, 101), "MyTimeTick", attr)

local attr = opcua.VariableAttributes.new()
attr.displayName = opcua.LocalizedText.new("en_US", "My Property DisplayName")
attr.description = opcua.LocalizedText.new("en_US", "My Property Description")
attr.value = opcua.Variant.new(8.8)
--[[
attr.writeMask = opcua.WriteMask.ALL
attr.userWriteMask = opcua.WriteMask.ALL
]]
attr.accessLevel = opcua.AccessLevel.RW
--attr.userAccessLevel = opcua.AccessLevel.READ ~ opcua.AccessLevel.READ
local myprop = newobject:addVariable(opcua.NodeId.new(idx, 97), "MyProperty", attr)
myprop.displayName = opcua.LocalizedText.new("en_US", "AAAAAAAAAAAAA")

local root = server:getRootNode()
print("Root node is", root)
print("Children are:")
for _, v in ipairs(root:getChildren()) do
	print("\t", v)
end

counter = 0

print("Ctrl-C to exit")

server:addCallback(function()
	counter = counter + 1
	local n = opcua.DateTime.now()
	mytick.value = opcua.Variant.datetime(n)
	print('callback', mytick.value:asDateTime(), n)
end, 1000)


server:startup()
while true do
	if counter > 100 then
		break
	end
	server:run_once(false)
	--local val = opcua.DataValue.new(counter)
	--val:SetSourceTimestamp(opcua.DateTime.Current()) --FromTimeT(timestamp)
	--val:SetServerTimestamp(opcua.DateTime.Current()) --FromTimeT(timestamp)
	--myvar.DataValue = val
end

server:shutdown()

--[[
server:run()
print('run finished')
print(server.running)
]]--
