rollkit
========
ESP IDF components for implementing a HomeKit accessory.

## Quick Start
1. In project cmake add `include(<path_to_rollkit>/CMakeLists.txt)`
2. Add `rollkit` to required components
3. Implement your Accessory

You can also look at the `simple_switch` in the examples folder to get an idea of how to implement an Accessory from scratch.

## Details
Rollkit will handle all the background communication between the HomeKit controller and your code. Currently rollkit will take two functions for reading and writing to the state of your device.

*Note: rollkit doesn't currently support updating state externally from event sources other than the paired controlling device - this functionality is in the works*

### Implementing an Accessory
*Some general notes from having gone through the process*

Likely the best place to start is by getting a copy of the HomeKit Accessory Protcol specification. It's *free* ish and will serve as a decent document to get info on what Services are available and what Services require what Characteristics.

Knowing this will be key if you want the iOS controller to properly display your Accessory in the iOS HomeKit app. Rest assured if you're seeing issue where the Accessory is listed as "Not Responding" then it is likely due to a miss-configuration on the Accessory side.

If you're having trouble implementing an Accessory and it's not showing up in the HomeKit app it's time to consult `Console` on a Mac. Once you have the device plugged in via USB and configured with Xcode you should be able to start viewing logs from the device. A simple filter for the `homed` process will help narrow down the messages you have to look at.

### Component Breakdown
 - `external` - Just houses code for mongoose and the json lib
 - `libtommath` - Used for all the big integer math
 - `srp` - My C++ implementation of Stanford's SRP protocol
 - `rollkit` - All the code related to implementing a HomeKit Accessory

### Caveat
I am __not__ a cryptography expert; as such I make __no__ guarantees about the security of any Accessories implemented with this library. I did have to implement some of the crypto for this library by hand. For those elements I have added unit tests from RFC test vectors to help ensure correctness. Beyond that however, I am out of my depth.

## TODO
 - [ ] External event feedback to controllers
 - [ ] Better Accessory sanity checking
 - [ ] Easier DSL for configuring accessories
