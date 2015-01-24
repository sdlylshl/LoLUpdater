//
//  ViewController.swift
//  LoLUpdater
//
//  Created by David Knaack on 31.12.14.
//
//

import Cocoa

class ViewController: NSViewController {

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
		
		let userDefaults = NSUserDefaults.standardUserDefaults()
		if let path = userDefaults.valueForKey("defaultPath") as? String {
			pathInput.stringValue = path
		}
		
    }

    

    override var representedObject: AnyObject? {
        didSet {
        // Update the view, if already loaded.
        }
    }

    @IBOutlet weak var progressBar: NSProgressIndicator!
    @IBOutlet weak var installOrRemove: NSMatrix!
    @IBOutlet weak var pathInput: NSTextField!
    @IBOutlet weak var findLoLButton: NSButton!
    
    @IBAction func findLoLButtonClick(sender: NSButton) {
		let openPanel = NSOpenPanel()
		
		openPanel.allowedFileTypes = ["app"]
		
		openPanel.allowsMultipleSelection = false
		openPanel.canChooseDirectories = false
		openPanel.canCreateDirectories = false
		openPanel.canChooseFiles = true
		openPanel.beginSheetModalForWindow(self.view.window!, completionHandler: { (result) -> Void in
			if result == NSFileHandlingPanelOKButton {
				self.pathInput.stringValue = openPanel.URL!.path!
			}
		})
		
    }
    @IBAction func patchButtonClick(sender: NSButton) {
		let fm = NSFileManager.defaultManager()
		
		let path = pathInput.stringValue.isEmpty ? pathInput.stringValue : pathInput.placeholderString!
		
		// TODO: proper error reporting and validation
		if !path.hasSuffix("app")
		{
			return
		}
		
		setDefaultPath(path)
		
		sender.enabled = false
        installOrRemove.enabled = false
        pathInput.enabled = false
        findLoLButton.enabled = false
        
        sender.title = "Working…"
        progressBar.hidden = false
		
		if installOrRemove.stringValue == "Install"
		{
			install(path)
		} else {
			remove(path)
		}
    }
	
	func install(path: String) {
		
		
		
		
	}
	
	func remove (path: String)
	{
	
	}
	
	func download(url: String, to: String) -> String? {
		// not finished yet
		let requestUrl = NSURL(string: url)
		let request = NSURLRequest(URL: requestUrl!)
		
		var download = NSURLDownload(request: request, delegate: nil)
	
		
		
		
		return nil;
	}
	
	func copy(from: String, to: String, err: NSErrorPointer = nil) {
		let fm = NSFileManager.defaultManager()
		fm.copyItemAtPath(from, toPath: to, error: err)
	}
	
	func setDefaultPath(path: String) {
		var userDefaults = NSUserDefaults.standardUserDefaults()
		userDefaults.setValue(path, forKey: "defaultPath")
		userDefaults.synchronize()
	}
	
	func highestVersionNumer(path: String) -> String {
		// TODO
		let fm = NSFileManager.defaultManager()
		return ""
	}


}

