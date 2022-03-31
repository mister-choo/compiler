# A very short history of PLC programming platforms

## A quick look at the past helps to shine the light on where programming software is today

By Tom Stevic, contributing editor

May 05, 2017

The actual programming language used to enter the commands into the PLC memory is referred to as instruction list (IL) or statement list (STL). The IL language is very similar to assembly language used in PC programming. Finally, the program would be entered in the controller’s memory using a numeric keypad.

In the later 1970s, inexpensive microprocessors became available in the marketplace, giving birth to the personal computer (PC). PLC manufacturers also used these microprocessors to develop graphical programming devices. These devices allowed a ladder-logic program to be entered using the graphical representation of normally open and normally closed contacts, output coils, timers, counters and other data manipulation and movement instructions. The programming terminals were single purpose, meaning they could only be used with a single manufacturer’s controllers. Compared to a modern laptop, the programming terminals were very large and very expensive, costing many times the cost of the controllers they programmed.

In the 1980s, software to program the major PLCs of the era became available. Interestingly, most of this software was produced by third party companies such as Taylor Software, ICOM, and others, and not the PLC manufacturers themselves.

Other software-only companies were bought by the PLC manufacturers, changed focus, or went out of business and are no longer around in their original form, however many of the original concepts and methods from the older software remain in current products.

Some of these original PLC software programming packages worked with multiple hardware platforms, resulting in a consistent look and feel across manufacturers. The documentation and reporting capabilities of this early DOS-based software was far superior to what the expensive OEM programming terminals provided. The PLC manufacturers recognized the success of the third-party software offerings and began to create and market competing products.

As a result, these third-party software providers changed hands many time. Taylor Software was purchased by Total Control Products, which which was later aquired by GE Fanuc. Schneider Electric also purchased parts of Taylor Software that included the Modicon components. ICOM programming software became a part of Rockwell Software.

[![ALSO READ: Advantages of simplified programming](https://no-cache.hubspot.com/cta/default/450117/6df429fe-4463-4937-801a-5ee3609b0220.png)](http://cta-redirect.hubspot.com/cta/redirect/450117/6df429fe-4463-4937-801a-5ee3609b0220)

### IEC 61131-3

> “IEC states, to be compliant, a programming package must support at least one of the five languages.”

In addition to the availability of relatively low-cost PCs and PLC programming software written for the PC, another significant development took place in the 1980s. Today, most major manufacturers are producing software development platforms that are compliant with the International Electrotechnical Commission (IEC) 61131-3 specification. This standard was started in 1982 and published in 1993 as IEC 1131 International Standard for Programmable Controllers.

As noted in the IEC standard, it was issued, in part, “to define, for each of the most commonly used programming languages, major fields of application, syntactic and semantic rules, simple but complete basic sets of programming elements, applicable test and means by which manufacturers may expand or adapt those basic sets to their own programmable controller implementations.”

If you are wondering where the ‘6’ used in the modern standard name came from, an explanation is here: (www.controldesign.com/61131). IEC 61131 has 10 parts covering various aspects of factory automation controllers. IEC 61131-3 is the part that covers programming.

The IEC 61131-3 standard is important because it brings a consistency to all software products conforming to the standard. For example, a standard IEC-defined function in a Brand-X PLC works the same in a Brand-Y PLC. A valuable result of this standardization is that functions and function blocks look the same and have the same defined inputs and outputs, regardless of the brand of integrated development environment (IDE) or the user-interface desktop, thus allowing an engineer or maintenance person to quickly understand logic and program flow from any manufacturer’s software.

The standard defines five programming languages: LD, function block diagram (FBD), sequential function chart (SFC), structured text (ST) and IL. IEC 61131-3 defines standard data types, naming conventions, program flow and various other programming elements. When a PLC OEM states that its programming software complies with IEC 61131-3, that usually means that the software complies with parts of the standard. IEC states, to be compliant, a programming package must support at least one of the five languages. CoDeSys may be the only programming platform that supports all five.

PLC manufacturers and users may also add custom extensions, function blocks and data types to the standard; this means that the seamless transfer of projects between platforms is not quite a reality, even if the platforms are IEC 61131-3 compliant. IEC 61131-10 defines XML file exchange formats. However, it is still a work in progress. Hopefully, we will have a mechanism to transfer projects across platforms one day.

### Automation suites

When purchasing a modern piece of control hardware, very often programming or setup software is available to assist the programming engineer. A trend among major manufacturers is to offer a software suite to provide a shared development interface for the products supported by the suite. After launching the suite, the designer can work with several product families, such as a PLC, an HMI and a motion controller, without starting separate software packages.

Some of these packages are nothing more than a wrapper that launches the standalone software. These packages have little or no integration between the programs. Every data point entered in one package needs to be copied or exported/imported from the first program to the second.

A handy feature that may be available is the ability to launch something like a DeviceNet configuration tool with the PLC project properties inherited. If modifications to a PLC project are made, launching the DeviceNet configuration tool with the appropriate project network file makes it easier to update both devices at the same time.

Truly integrated automation suite software developed as a single package can reduce the workload of a programmer. Starting with a PLC or PAC definition, the programmer can add other supported devices, such as a servo controller or safety controller and the software automatically creates tags consumed and produced by the added devices. When creating the new tags, the software automatically creates the communication path and the data type definition.

A drawback to using software suites is they seldom support another manufacturer’s products. A device made by some other manufacturer still needs to be defined and added to the project by hand. Another disadvantage occurs when the manufacturer stops supporting individual software packages for some devices, requiring the purchase of a high-priced software package to support an inexpensive control product.

Automation software is constantly evolving. It certainly does much more than the early DOS products. It is now possible to learn one brand and very quickly change to a different brand without a steep learning curve. It is easier on the eyes than the text-based products from years long gone. However, at times, such as when a production manager wants his machine running now, I long for a programming device that would start, load the software and PLC project and then let me find the problem in less time than it takes my current laptop to boot to its operating system.