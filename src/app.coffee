app = require "app"
BrowserWindow = require "browser-window"

path = require "path"

# Report crashes to our server.
# TODO: Use our own server for crash reporting.
require("crash-reporter").start()

# Keep a global reference of the window object, if you don't, the window will
# be closed automatically when the javascript object is GCed.
mainWindow = null

# Quit when all windows are closed.
app.on "window-all-closed", ->
  app.quit()


# This method will be called when atom-shell has done everything
# initialization and ready for creating browser windows.
app.on "ready", ->
  # Create the browser window.
  mainWindow = new BrowserWindow(
    "width": 800
    "height": 600
    "min-width": 400
    "min-height": 300
    "frame": false
    "direct-write": true
  )
  # and load the index.html of the app.
  mainWindow.loadUrl path.join __dirname, "index.html"
  # mainWindow.openDevTools()

  mainWindow.on "close", ->
    app.quit()

  ipc = require "ipc"
  ipc.on "minimize", ->
    mainWindow.minimize()

  ipc.on "patch", (e, cmdArg, LoLPath) ->
    cp = require "child_process"
    options =
      cwd: LoLPath
    cp.execFile path.join( __dirname, "LoLUpdater.exe"), [cmdArg], options
  return
