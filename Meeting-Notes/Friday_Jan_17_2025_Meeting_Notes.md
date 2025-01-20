**Meeting Notes taken by Mohammad during the initial meeting with Rahul (01-17-2025) @ 1pm**
**ATTENDENCE**:
	- Roy Kravitz
	- Nathan Fraly
	- Riley Cox
	- Xiang Li
	- Hao Yang Han
	- Mohammad Alshaiji

# Meeting Notes:	
The meeting is being held in order to discuss the scope of the project with Rahul:
- We shared with Rahul that in our research we were not able to find any open-source tools that would function with the Lattice Ultraplus or Crosslink FPGAs.
- Rahul informed us past student groups have been able to get the Yosys open-source toolchain running with a upduino board containing an iCE40 Ultraplus FPGA

## Three Phases:
As stated by Rahul the Project will consist of three phases.
In phase 1, we will create a "nice" end product; Potentially to be used in showcasing events
The end product will consist of the following:
	- A polished 3D-printed Enclosure
	- An LCD Screen
	- An Upduino Board with an iCE40 Ultraplus FPGA
	- Two buttons, one to control color changing, the other brightness/contrast
	
The display should be able to shift through 7-8 solid colors in the intial protype, to be stepped up to 10-15 colors by the end of phase 1.

A graphic of the proposed layout of product at the end of phase 1 is displayed below:
<pre>
		[PHASE 1]
+--------------------------------------------------+
|                                                  |
|                                                  |
|                                                  |
|                                                  |
|      .-~-.                    +--------------+   |
|     (  B1 )                   |              |   |
|      `-~-'                    |              |   |
|                               |     LCD      |   |
|                               |    SCREEN    |   |
|      .-~-.                    |              |   |
|     (  B2 )                   |              |   |
|      `-~-'                    +--------------+   |
|                                                  |
|                                                  |
|                                                  |
+--------------------------------------------------+
</pre>

In Phase 2, we are to upgrade the functionsality of the device so that it can display an image such as a smily face:

		 [PHASE 2]
   <pre>
+--------------------------------------------------+
|                                                  |
|                                                  |
|                                                  |
|                                                  |
|      .-~-.                    +--------------+   |
|     (  B1 )                   |              |   |
|      `-~-'                    |   --    --   |   |
|                               |              |   |
|                               |  |        |  |   |
|      .-~-.                    |   --------   |   |
|     (  B2 )                   |              |   |
|      `-~-'                    +--------------+   |
|                               Image Displayed    |
|                                                  |
|                                                  |
+--------------------------------------------------+
   </pre>
Rahul would like us to upgrade the product further to be able to display text on the display:

		[PHASE 3]
  <pre>
+--------------------------------------------------+
|                                                  |
|                                                  |
|                                                  |
|                                                  |
|      .-~-.                    +--------------+   |
|     (  B1 )                   |              |   |
|      `-~-'                    |    *TEMP:*   |   |
|                               |    *89 F*    |   |
|                               |              |   |
|      .-~-.                    |              |   |
|     (  B2 )                   |              |   |
|      `-~-'                    +--------------+   |
|                                Text Displayed    |
|                                                  |
|                                                  |
+--------------------------------------------------+
  </pre>
  
## QnA:

As our industry sponsor requires an end product that is polished enough to be showcased, we inquired about the power requirements and the low-power design techniques he had mentioned previously in our email communication.
- Rahul stated that by the end of Phase 1, the device must be battery powered but we can utilize a battery pack to accomplish this.
- He further clarified that the low-power design techniques and their documentation is to be utilized for the purposes of being able to compare the effect of two toolchains (open-source and lattice proprietery) on synthesis. 
- Specifically, an end result should be a visualization of different power profiles, and a comparison of the results of the open-source toolchain, vs Lattice's toolchain.
- Some techniques suggestions are power gating, turning the LCD off at certain intervals etc. 

We questioned Rahul about the purpose of having us complete this project, i.e. what is Lattice's intended benefit from this project. 
Rahul replied that:
- Lattice is really interested in a comparison of the two toolchains, specifically a comparison of the ease of use of the toolchains, the implementation, and a comparison of the results of their output (power profiles etc.)
- At the end of our project, they would like a recommendation from the team based on quantifiable results. 
- The device is to serve as a tool to show potential costumers a comparison of FPGAs to Microcontrollers so that they may show them that FPGAs are not more expensive in space requirements, nor in ease of use.

We asked Rahul if the github should be started by us or if it will be provided, and if there are documentation standards we should follow. He replied:
- The team is responsible for starting a github repository
- The Lattice website contains a document on their general standards

Rahul further wished to clarify that:
- He will ask the other Crosslink team for help with the toolchain should we need it
- We may utilize IP (cores, SPI Bus), such as those avaliable on Opencores.com, ensuring that the same code is fed into the two toolchains, allowing for a viable comparison to be made
- We can explore RISC-V or RTL implementations
- Some metrics we can utilize are: Power, ease of use. 
- For debugging, we should be able to display messages on the LCD display. 

Roy suggested that if the intent is for the device to be able to function as a light-weight debugging tool, it would make sense for us to be able to display text on the LCD before we display images.
Rahul agreed and so order of the phases was altered as follows:


                   [PHASE 1]
<pre>
+--------------------------------------------------+
|                                                  |
|                                                  |
|                                                  |
|                                                  |
|      .-~-.                    +--------------+   |
|     (  B1 )                   |              |   |
|      `-~-'                    |              |   |
|                               |     LCD      |   |
|                               |    SCREEN    |   |
|      .-~-.                    |              |   |
|     (  B2 )                   |              |   |
|      `-~-'                    +--------------+   |
|                                                  |
|                                                  |
|                                                  |
+--------------------------------------------------+
</pre>

For phase 2, Rahul would like us to upgrade the product to be able to display text on the display:

		[PHASE 2]
<pre>
+--------------------------------------------------+
|                                                  |
|                                                  |
|                                                  |
|                                                  |
|      .-~-.                    +--------------+   |
|     (  B1 )                   |              |   |
|      `-~-'                    |    *TEMP:*   |   |
|                               |    *89 F*    |   |
|                               |              |   |
|      .-~-.                    |              |   |
|     (  B2 )                   |              |   |
|      `-~-'                    +--------------+   |
|                                Text Displayed    |
|                                                  |
|                                                  |
+--------------------------------------------------+
</pre>

In Phase 3, we are to upgrade the functionsality of the device further so that it can display an image such as a smily face:

		[PHASE 3]
<pre>
+--------------------------------------------------+
|                                                  |
|                                                  |
|                                                  |
|                                                  |
|      .-~-.                    +--------------+   |
|     (  B1 )                   |              |   |
|      `-~-'                    |   --    --   |   |
|                               |              |   |
|                               |  |        |  |   |
|      .-~-.                    |   --------   |   |
|     (  B2 )                   |              |   |
|      `-~-'                    +--------------+   |
|                               Image Displayed    |
|                                                  |
|                                                  |
+--------------------------------------------------+
</pre>
Rahul's final note was that:
- He expects weekly progress reports and constant communication
- Nathan is to serve as his point of contact
- We need to regularly think about where we are and our path to the end-product
