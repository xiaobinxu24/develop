

## Program Interactive
You know, we need connect with other program someday. So This program need receive
some command and data from other program. 

##### How we implement it ?

We create a unix socket, let other program send command to it.

Unix socket name is `/tmp/kfmng.sock`.

```

+---------------+        +------------------+        +------------------+
| other program | -----> | /tmp/kfmng.sock  | -----> | Receive and deal |
+---------------+        +------------------+        +------------------+

```

##### Format:
```
+---------+-+--------+-+-----+-+-----+-+------+--------------------------
| version | | sender | | seq | | cmd | | args |
+---------+-+--------+-+-----+-+-----+-+------+--------------------------
           |
           v
         space
```
- version: 0 (int)
- sender: Which program send this command (string)
- seq: Cmd sequence, the same command if next seq same with previous.(unsigned int)
- cmd: Command value (program interactive define it) (string)
- args: Args with command, (max 1024 bytes) (string)

##### Warn:
- All ascii string format, utf-8.
- Unsupport space in each command
- Args: don't with space, '\n', and other special marks, use `_` if you need differentiate them.
  Ex: `test1_test2_do`

##### Usage:
- Now only support `Network Optimization`.

#### Network Optimization
##### What to do with this option ?

When user click `NetworkOptimization` on the page, program need re-queryip.

##### How to implement in program ?
```
cmd: NetworkOptimization
```
Change `/etc/kfconfig/kfIPProperties.conf` -> `config` to false. and program need requery ip.

#### All function defined here

```
+---------+-+--------+-+-----+-+-----+-+------+--------------------------
| version | | sender | | seq | | cmd | | args |
+---------+-+--------+-+-----+-+-----+-+------+--------------------------

   0          xxx         0     NetworkOptimization   ""
   0          xxx         0     OutsideControl        mode=1/2/3

```

----------------------------------------------------------------------------------------------
## IfEnableOutsideWithPage

##### Start

+----------------+  DisableOutSide   +---------------------------+
|     config     | ----------------> | KF_ST_WAIT_ENABLE_OUTSIDE |
+----------------+                   +---------------------------+
  |
  | EnableOutSide
  v
+----------------+
|    queryip     |
+----------------+

1. Check config value `/etc/kfconfig/kfconfig`, `mode` when system start.
2. Enter `KF_ST_WAIT_ENABLE_OUTSIDE` mode if disable Outside.
3. Do next others.

##### Running

     ChangeOutsideMode   +----------------+  DisableOutSide   +--------------+     +---------------+
    -------------------> |     socket     | ----------------> | OnlineNotify | --> | UploadTraffic |
                         +----------------+                   +--------------+     +---------------+
                           |
                           | EnableOutside
                           v
                         +----------------+
                         | SetEnableMark  |
                         +----------------+


1. Lua will send unixsocket to program for ChangeOutside mode when user click disableOutside.
2. SendNotify Define string format `%Y-%m-%d-%h-%m-%s-UserDisableOutside`, and level is `WARNING`. (Don't use this if 2.1 is used)
2.1 Set mark in OnlineNotify if Need use this style, it will disable SendNotify
3. Just set enable outside mark when receiver socket command (donothing, If EnableOutside)
4. Goto OnlineNotify if test outside mode is 1, then goto ClearTraffic, UploadTraffic
5. Disable Bind and CheckOutside if outside mode is 1 when system start.
6. Goto KF_ST_UNINIT when mode from 1 to 2,3.

----------------------------------------------------------------------------------------------
## IfNetwork OK

#### Why we need this
As some scene, we didn't know send information error, as network disconnect, or our server failed.
We need check it, so we add this.

#### How to implement it
In program, we add a class to check network, in process, we add KF_ST_CHECK_NETWORK, go in this status when we send data failed.
Go on until we check the network is ok.

