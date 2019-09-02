-----------
-- Client example
--

local opcua = require 'opcua'

local client = opcua.Client.new()

local config = client.config
config:setTimeout( 5000)
config:setSecureChannelLifeTime( 10 * 60 * 1000 )

local state_callback = function(client, state)
	print(client, state)
end

client:setStateCallback(state_callback)

--local r, err = client:connect("opc.tcp://127.0.0.1:4840")
local r, err = client:connect("opc.tcp://172.30.0.187:38133")
print(r, err)
print("StatusCodeName", opcua.getStatusCodeName(r))

local root = client:getRootNode()
print("Root:", root, root.browseName)
print("Server:", client.serverNode)

local id = opcua.NodeId.new(0, 2258)
local node = client:getNode(id)
print( 'getNode', node)

--[[
while node do
	local dv = node.dataValue
	print(dv.value:asString())
end
]]--

local sub_id, err = client:createSubscription(function(mon_id, data_value, sub_id)
	print(mon_id, data_value.value:asDateTime(), sub_id)
end)

if not sub_id then
	print("Error Sub", err)
end

local mon_id, err = client:subscribeNode(sub_id, id)
if not mon_id then
	print("Monitor failed", err)
end

while true do
	client:run_iterate(1000)
end

client:disconnect()
print('DONE')

