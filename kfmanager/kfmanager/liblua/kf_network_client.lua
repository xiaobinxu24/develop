--
-- User: v
-- Date: 16-4-28
-- Time: 下午4:28
--

--print(package.path)
--package.path = package.path .. "/usr/lib/lua/luci/model/?.lua;"
--package.path = package.path .. "/usr/?.lua;"

--package.path = package.path .. ";/tmp/kf/kfmanager/?.lua;/tmp/kf/kfmanager/log/?.lua;"

-- Create network client for send and recv data with unix socket.

local M = {}


socket = require "socket"
socket.unix = require "socket.unix"


function  generate_msg(sender,cmd,ag)
    local version = "0"
    --local seq=  os.time()
    local seq = 1

    local msg = version .. " " .. sender  ..  " " .. seq .. " "  .. cmd  .. " " .. ag

    return msg;
end

function client_send(sender,cmd,ag)
    c = socket.unix()
    c:connect("/tmp/kfmng.sock")
    -- c:send("0 sender seq NetworkOptimization ag")
    local data = generate_msg(sender,cmd,ag)
    c:send(data)
    socket.select(nil,nil,1)
end


--M.netClentSend = client_send(arg[1],arg[2],arg[3])
M.netClientSend = client_send
return M
