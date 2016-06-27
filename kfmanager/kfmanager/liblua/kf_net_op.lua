--
-- Module: ${PACKAGE_NAME}
-- User: v
-- Date: 16-3-21
-- Time: 上午9:29
--

function kf_ping_parse(pingresult)

    local indx = string.find(pingresult, "statistics"), string.len(pingresult)-1
    if not indx then return nil end

    local result = string.sub(pingresult, indx,  string.len(pingresult)-1)
    if not result then return nil end

    pkgloss_r = ", (%d+)%%%spacket%sloss"
    local pkglossrate = string.match(result, pkgloss_r)
    if not pkglossrate then return nil end

    pkglossstr = "(%d+.%d+)/(%d+.%d+)/(%d+.%d+)"
    local min, avg, max = string.match(result, "(%d+.%d+)/(%d+.%d+)/(%d+.%d+)")

    if min then
        print("min: " .. min)
    end
    if avg then
        print("avg: " .. avg)
    end

    if max then
        print("max: " .. max)
    end


--    print("pkglossrate " .. pkglossrate .. ", min " .. min .. ", avg: " .. avg .. ", max " .. max)
    return pkglossrate, min, avg, max
end

function kf_ping_server(serverip, distance, belonggroup, latitude, longitude)
    --print("lua:server ip = " .. serverip)
    if not serverip then
        return nil
    end
    local pingstr = string.format("ping -i 1 -c 3 %s", serverip)
    local pingret = io.popen(pingstr)
    local pingresult = pingret:read("*all")
    pingret:close()

    return kf_ping_parse(pingresult)
end


