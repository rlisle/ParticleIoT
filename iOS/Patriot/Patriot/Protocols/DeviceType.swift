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
    case Fan = "F"
    case Light = "L"
    case Motor = "M"
    case NCD8Relay = "8" //placeholder
    case Presence = "P"
    case Relay = "R"
    case Switch = "S"
    case TempHumidity = "T"
    case Ultrasonic = "U"
}
