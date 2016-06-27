--print(package.path)
--package.path = package.path .. "/usr/lib/lua/luci/model/?.lua;"
--package.path = package.path .. "/usr/?.lua;"
uci = require "luci.model.uci"
_uci_real  = uci.cursor()


--return command result
function exec(command)
    local pp = io.popen(command)
    local data = pp:read("*a")
    pp:close()
    return data
end

--get meminfo
function kf_get_meminfo()
    --get total meminfo
    local total = exec(" cat /proc/meminfo  | grep MemTotal | cut -d ':' -f2 | sed \"s/'//g\" ")
    for w in string.gmatch(total, "%d+") do
        total = w
    end
    --get free meminfo
    local free = exec(" cat /proc/meminfo  | grep MemFree | cut -d ':' -f2 | sed \"s/'//g\" ")
    for w in string.gmatch(free, "%d+") do
        free = w
    end
    if not total then return nil end
    if not free then return nil end
    total = tonumber(total)
    free = tonumber(free)
    return total, free
end

--get waninfo
function kf_get_waninfo()
    local ntm = require "luci.model.network".init()
    local wan = ntm:get_wannet()
    if wan == nil then
        return nil
    else
        local wanip = wan:ipaddr()
        local dnsaddrs = wan:dnsaddrs()
        dns1 = dnsaddrs[1]
        dns2 = dnsaddrs[2]
        local netmask = wan:netmask()
        local gateway = wan:gwaddr()
        return wanip, dns1, dns2, netmask, gateway
    end
end
--get starttime
function get_startup_time()
    local f = io.open("/proc/uptime")
    local data = f:read("*line")
    local uptime =  string.match(data, "(.+%s)")
    f:close()
    tm = os.date("%Y-%m-%d %H:%M:%S", os.time()-uptime)
    return tm
end





