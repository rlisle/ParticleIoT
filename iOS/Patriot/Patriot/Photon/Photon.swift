//
//  Photon.swift
//  Patriot
//
//  This class provides the interface to a Photon microcontroller.
//
//  The Photon will be interrogated to identify the devices
//  that it implements using the published variables:
//
//      deviceNames     is a list of all the devices exposed on the Photon
//                      Format is: t:name=#
//                          where t = 'l' light, 's' switch, 'c' curtain
//                                name = device name (eg. officeceiling)
//                                and # = device value 0-255 (percent 0-100)
//
//      value(name: String) return the current device value
//      type(name: String) returns the device type
//
//  This file uses the Particle SDK:
//      https://docs.particle.io/reference/ios/#common-tasks
//
//  Created by Ron Lisle on 4/17/17
//  Copyright © 2016, 2017 Ron Lisle. All rights reserved.
//

import Foundation
import Particle_SDK

class Photon: HwController, Identifiable
{
    let uninitializedString = "uninitialized"
    let id = UUID()
    
    var devices: [DeviceInfo] = []      // Cached list of device names exposed by Photon
    
    var delegate: PhotonNotifying?      // Notifies manager when status changes
    
    internal let particleDevice: ParticleDevice! // Reference to Particle-SDK device object
    
    var name: String
    {
        get {
            return particleDevice.name?.lowercased() ?? "unknown"
        }
    }
    
    required init(device: ParticleDevice)
    {
        particleDevice  = device
    }

    /**
     * Refresh is expected to be called once after init and delegate is set
     */
    func refresh()
    {
        refreshDevices()
    }
}

extension Photon    // Devices
{
    func refreshDevices()
    {
        devices = []
        readVariable("Devices") { (result) in
            if let result = result {
                self.parseDeviceNames(result)
            }
        }
    }
    
    
    private func parseDeviceNames(_ deviceString: String)
    {
        // Format is t:name=#, ...
        print("parseDeviceNames: \(deviceString)")
        let items = deviceString.components(separatedBy: ",")
        for item in items
        {
            print("item: \(item)")
            let types = item.components(separatedBy: ":")
            if types.count == 2 {
                let deviceType = DeviceType(rawValue: types.first?.first ?? "?") ?? DeviceType(rawValue: "?")! // c, l, s, etc.
                let values = types[1].components(separatedBy: "=")
                if values.count == 2 {
                    let name = values[0]
                    let percent = Int(values[1]) ?? 0
                    
                    print("Device = \(name), type: \(deviceType.rawValue), percent: \(percent)")
                    let deviceInfo = DeviceInfo(name: name, type: deviceType, percent: percent)
                    self.devices.append(deviceInfo)
                    self.delegate?.device(named: self.name, hasDevices: self.devices)
                }
            }
        }
    }
}

extension Photon        // Read variables
{
    func readVariable(_ name: String, completion: @escaping (String?) -> Void)
    {
        guard particleDevice.variables[name] != nil else
        {
            print("Variable \(name) doesn't exist on photon \(self.name)")
            completion(nil)
            return
        }
        particleDevice.getVariable(name) { (result: Any?, error: Error?) in
            completion(result as? String)
        }
    }
    
    func callFunction(name: String, args: [String], completion: @escaping (Int?) -> Void)
    {
        particleDevice.callFunction(name, withArguments: args) { (result: NSNumber?, error: Error?) in
            completion(result as? Int)
        }
    }
}
