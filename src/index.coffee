addEventListener "DOMContentLoaded", ->
  $ = document.querySelector.bind(document)
  $$ = document.querySelectorAll.bind(document)

  remote = require "remote"
  ipc = require "ipc"

  exitApp = ->
    app = remote.require "app"
    app.quit()

  minimizeApp = ->
    ipc.send "minimize"

  $("#patch").addEventListener "click", (e) ->
    arg = $("paper-radio-button.core-selected").dataset.arg
    LoLPath = $("lol-finder").value
    controlElements = $$("core-icon-button.exit, paper-radio-button, #patch,
                          lol-finder /deep/ paper-input, lol-finder /deep/
                          paper-button")

    c.disabled = "disabled" for c in controlElements
    e.srcElement.innerText = "Patching…"

    localStorage["LoLPath"] = LoLPath

    ipc.send "patch", arg, LoLPath


  exit.addEventListener "click", exitApp for exit in $$(".exit")

  $(".minimize").addEventListener "click", minimizeApp

  $(".bug").addEventListener "click", ->
    shell = require "shell"
    shell.openExternal "https://github.com/Loggan08/LoLUpdater/issues"
