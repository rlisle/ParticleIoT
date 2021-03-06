//
//  ContentView.swift
//  Patriot
//
//  Created by Ron Lisle on 5/25/21.
//

import SwiftUI

struct ContentView: View {
    
    @State private var needsLogin = true
    
    var body: some View {

        VStack {
//            Menu("Activities") {
//                List(PhotonManager.shared.activities) {
//                    Text($0.name)
//                }
//            }
            
//            Menu("Devices") {
//                List(PhotonManager.shared.devices) {
//                    Text($0.name)
//                }
//            }
//
//            Menu("Photons") {
//                //TODO: use enumeration instead of hardcoding
//                List {
//                    Text(PhotonManager.shared.photons["FrontPanel"].name)
//                    Text(PhotonManager.shared.photons["RearPanel"].name)
//                }
//            }
            
            Menu("Settings") {
                Button("Log Out") {
                        needsLogin = true
                    }
            }
        }
        .sheet(isPresented: $needsLogin) {
            LoginView(needsLogin: $needsLogin)
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
