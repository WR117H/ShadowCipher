# ShadowCipher

<p align="center">
  <img width="220" height="102" src="https://github.com/WR117H/ShadowCipher/assets/97615989/b993fda5-5655-4a4e-96eb-31e3f90dd756" >
</p>
<p align="center">
  <img src="https://github.com/WR117H/ShadowCipher/assets/97615989/1d2341fd-8c21-40e1-9246-126a74c39225">

</p>

An rf tool based on CC1101 module and esp8266/rp2040.
Allows using CLI to control CC1101 board over USB interface. Putty or any other serial terminal can be used.

### YardStick One
![Yard_stick_one_bundle_bg](https://github.com/WR117H/ShadowCipher/assets/97615989/b6cb4780-be03-4f8c-9433-e06b30d6bf9b)
</br>
It has similar functionality to YardStick One but is cheaper and does not need specialized software. Allows for RF jamming and replay attacks as well. It has RAW recording/replaying function which works exactly the same as in the Flipper Zero. Additional function is Radio Chat communicator

### How to build one?
> <details><summary><code>D.I.Y Method</code></summary><ul>
>   <br>
>   <li>Just connect an CC1101 to ESP8266 by the schematic link below</li>
>   <br>
>   <li>https://github.com/WR117H/ShadowCipher/assets/97615989/6da47cfb-2334-4604-b99e-271cfd8e5fa5</li>
>   <br>
>   <li>by using `python setup.py --port [port] --baud [baud]` You can just update or install the ShadowCipher firmware on your device </li>

> </ul></details>
> <details><summary><code>ShadowCipher pcb</code></summary><ul>
>   <br>
>   <li>Comming soon ...</li>
> </ul></details>

### Useage


### Commands
Following attacks are available :
`capture: It records the raw data frequency and it can play it after recording
sniffing: It can read the signals
scan: It can find the frequency
graph: It shows the rssi graph on the display
`

