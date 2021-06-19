//
//  DeviceType.swift
//  Patriot
//
//  Created by Ron Lisle on 6/2/18.
//  Copyright © 2018 Rons iMac. All rights reserved.
//

import Foundation

enum DeviceType: Character {
    case Unknown = "?"
    case Fan = "f"
    case Light = "l"
    case Motor = "m"
    case NCD8Relay = "8" //placeholder
    case Presence = "p"
    case Relay = "r"
    case Switch = "s"
    case TempHumidity = "t"
    case Ultrasonic = "u"
}
