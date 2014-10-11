addEventListener "DOMContentLoaded", ->
  remote = require "remote"
  ipc = require "ipc"

  exitApp = ->
    app = remote.require "app"
    app.quit()

  minimizeApp = ->
    ipc.send "minimize"

  document.getElementById("patch").addEventListener "click", ->
    arg = document.querySelector("paper-radio-button.core-selected").dataset.arg
    LoLPath = document.querySelector("lol-finder").value
    document.querySelector(".patch-dialog").toggle()
    localStorage["LoLPath"] = LoLPath
    ipc.send "patch", arg, LoLPath


  for exit in document.querySelectorAll(".exit")
    exit.addEventListener "click", exitApp

  document.querySelector(".minimize").addEventListener "click", minimizeApp

  document.querySelector(".bug").addEventListener "click", ->
    shell = require "shell"
    shell.openExternal "https://github.com/Loggan08/LoLUpdater/issues"
