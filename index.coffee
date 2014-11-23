spark = require 'spark'
{spawn} = require 'child_process'

runApplescript = (script) ->
  as = spawn "osascript", ["-ss"]
  as.stdin.write script
  as.stdin.end()

spark.on 'login', ->
  spark.listDevices().then (devices) ->
    core = devices[0]
    spark.getEventStream 'release', 'mine', (data) ->
      data = data.replace /^[^\{]*(\{.*\})[^\}]*$/, "$1"
      try
        data = JSON.parse data
        pin = data.data
        console.log "Pushed #{pin}"
        keys =
          D1: "key code 126"
          D2: "key code 124"
          D3: "key code 125"
          D4: "key code 123"
          ALL: "key code 48 using command down"
        script = """
          tell application "System Events"
            #{keys[pin]}
          end tell
          """
        runApplescript script

spark.login
  username: process.env.SPARK_USERNAME
  password: process.env.SPARK_PASSWORD
