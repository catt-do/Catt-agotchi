# ˖⁺. ༶ ❤︎ ⋆˙⊹ 𐦍 ˖⁺. ༶ ❤︎ ⋆˙⊹Catt-agotchi Design˖⁺. ༶ ❤︎ ⋆˙⊹ 𐦍 ˖⁺. ༶ ❤︎ ⋆˙⊹

Since I'm making this for fun I wasn't sure if I needed to have proper documentation or not so here's some stuff I
would put into a _hypothetical_ design document.

-------˖⁺. ༶ ❤︎ ⋆˙⊹ 𐦍 ˖⁺. ༶ ❤︎ ⋆˙⊹-------
## ︎Intro

I wanted to make a Tamagotchi-like game, but instead of the digital pets they normally have, I decided that I wanted 
to make a cute "mini Catt (me)" so I did.

-------˖⁺. ༶ ❤︎ ⋆˙⊹ 𐦍 ˖⁺. ༶ ❤︎ ⋆˙⊹-------
# Functional Requirements

## Gameplay

♡ **The "Catt" living in the Catt-agotchi will have three stats, all of which decrease over time:**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Fullness_

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Happiness_

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Wellness_

♡ **The "Catt" will have three different states, dependent on stat values:**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Satisfied_: All stats are > 0

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Unsatisfied_: At least one stat is 0 but not all

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Dead_: Displayed when all stats are 0

♡ **Stats can be replenished by the following actions:**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Give Milk_: Increases fullness

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Give Heart_: Increases happiness

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Give Meds_: Increases wellness

-------˖⁺. ༶ ❤︎ ⋆˙⊹ 𐦍 ˖⁺. ༶ ❤︎ ⋆˙⊹-------

## Interface

♡ **There will be three sprites for the "Catt"s three different states:**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Happy Catt_: Two-frame idle animation of "Satisfied Catt" smiling while yapping

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Sad Catt_: Four-frame idle animation of "Unsatisfied Catt" crying

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _R.I.P. Catt_: Still image of "Dead Catt"s grave

♡ **There will be an icon for each of the three stat-replenishing actions:**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Milk Carton_: Icon representing the "Give Milk" action

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Heart_: Icon representing the "Give Heart" action

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ _Prescription Bottle_: Icon representing the "Give Meds" action

♡ **General Interface Elements:**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ be rectangles behind icons and stat bars in the corners of the screen

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ each icon highlighted when selected

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;♡ about stat bars

-------˖⁺. ༶ ❤︎ ⋆˙⊹ 𐦍 ˖⁺. ༶ ❤︎ ⋆˙⊹-------

## Other

♡cute case

♡small

♡cute

-------˖⁺. ༶ ❤︎ ⋆˙⊹ 𐦍 ˖⁺. ༶ ❤︎ ⋆˙⊹-------
# Components

## Hardware

♡ESP32-S3

♡ST7789 240x300 TFT display

-------˖⁺. ༶ ❤︎ ⋆˙⊹ 𐦍 ˖⁺. ༶ ❤︎ ⋆˙⊹-------

## Software

♡catt

♡driver

-------˖⁺. ༶ ❤︎ ⋆˙⊹ 𐦍 ˖⁺. ༶ ❤︎ ⋆˙⊹-------
# Architecture

put in diagram

-------˖⁺. ༶ ❤︎ ⋆˙⊹ 𐦍 ˖⁺. ༶ ❤︎ ⋆˙⊹-------