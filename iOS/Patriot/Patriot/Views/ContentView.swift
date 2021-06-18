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

        NavigationView {
//            Menu("Activities") {
//                List(PhotonManager.shared.activities, id: \.name) {
//                    Text($0.name)
//                }
//            }
            
//            Menu("Devices") {
//                List(PhotonManager.shared.devices, id: \.name) {
//                    Text($0.name)
//                }
//            }

//            Menu("Photons") {
                List(PhotonManager.shared.photons) {
                    Text($0.name)
                }
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
