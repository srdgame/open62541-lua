----------------
-- Test example

local opcua = require 'opcua'
--[[
opcua.setLogger(function(...)
	print(...)
end)
]]--
print(opcua.VERSION)

local securityMode = opcua.UA_MessageSecurityMode.UA_MESSAGESECURITYMODE_SIGNANDENCRYPT
print(securityMode)

local client = opcua.Client.new(securityMode, "certs/my_cert.der", "certs/my_private_key.der")
local config = client.config
config:setApplicationURI("aaaa")
print(client)
local r, err = client:connect_username("opc.tcp://10.0.0.117:49320", "Administrator", "Pa88word")
--local r, err = client:connect()
--local r, err = client:connect("opc.tcp://Administrator:Pa88word@172.30.1.153:49320")
--local r, err = client:connect("opc.tcp://172.30.1.153:49320")
print('connect result', r, err)
if r ~= 0 then
	print(opcua.getStatusCodeName(r))
end

local root = client:getRootNode()
print("Root:", root, root.browse_name)
print("Server:", client:getServerNode())

print (client:getNamespaceIndex("KEPServerEX"))

os.execute("sleep 10")
