-----------
-- Client example
--

local opcua = require 'opcua'
--[[
opcua.setLogger(function(...)
	print(...)
end)
]]--

--[[
local function connection_config_init(config)
	config.protocolVersion = 0
	config.sendBufferSize = 65535
	config.recvBufferSize = 65535
	config.maxMessageSize = 0
	config.maxChunkCount = 0
end
]]--

local client = opcua.Client.new()

local config = client.config
config:setTimeout( 5000)
config:setSecureChannelLifeTime( 10 * 60 * 1000 )

local r, err = client:connect("opc.tcp://192.168.0.100:4840")
print(r, err)
print("StatusCodeName", opcua.getStatusCodeName(r))

local root = client:getRootNode()
print("Root:", root, root.browseName)
print("Server:", client.serverNode)

local id = opcua.NodeId.new(0, 2258)
local node = client:getNode(id)
print( 'getNode', node)

while node do
	local dv = node.dataValue
	print(dv.value:asString())
end

client:disconnect()
print('DONE')

