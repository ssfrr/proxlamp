v 20081231 1
C 40000 40000 0 0 0 title-B.sym
C 44400 47000 1 0 0 fuse-1.sym
{
T 44600 47400 5 10 0 0 0 0 1
device=FUSE
T 44600 47200 5 10 1 1 0 0 1
refdes=F1
T 44600 47600 5 10 0 0 0 0 1
symversion=0.1
T 44400 47000 5 10 1 1 0 0 1
footprint=3AG_fuse
T 44400 47000 5 10 1 1 0 0 1
value=2A
}
C 43700 44000 1 90 0 hole-1.sym
{
T 43700 44000 5 10 0 1 90 0 1
device=HOLE
T 43100 44200 5 10 1 1 90 4 1
refdes=H3
T 43700 44000 5 10 1 1 0 0 1
value=AC_GND
T 43700 44000 5 10 1 1 0 0 1
footprint=hole_m3
}
C 43700 45800 1 90 0 hole-1.sym
{
T 43700 45800 5 10 0 1 90 0 1
device=HOLE
T 43100 46000 5 10 1 1 90 4 1
refdes=H2
T 43700 45800 5 10 1 1 0 0 1
value=AC_NEUT
T 43700 45800 5 10 1 1 0 0 1
footprint=hole_m3
}
C 43700 46800 1 90 0 hole-1.sym
{
T 43700 46800 5 10 0 1 90 0 1
device=HOLE
T 43100 47000 5 10 1 1 90 4 1
refdes=H1
T 43700 46800 5 10 1 1 0 0 1
value=AC_HOT
T 43700 46800 5 10 1 1 0 0 1
footprint=hole_m3
}
C 45700 44700 1 0 0 transformer-dualpri-dualsec.sym
{
T 46000 46000 5 10 0 0 0 0 1
device=transformer
T 46000 47200 5 10 1 1 0 0 1
refdes=T1
T 45700 44700 5 10 1 1 0 0 1
footprint=transformer-dualpri-dualsec
}
C 47800 46300 1 0 0 bridge-1.sym
{
T 48000 47300 5 10 1 1 0 0 1
refdes=U1
T 48000 47700 5 10 0 0 0 0 1
device=bridge
T 48000 48100 5 10 0 0 0 0 1
symversion=0.1
T 47800 46300 5 10 1 1 0 0 1
footprint=diode-bridge
}
N 45400 45800 45400 47000 4
N 45400 45800 45700 45800 4
N 43700 46000 45700 46000 4
N 45300 46000 45300 44800 4
N 45300 44800 45700 44800 4
N 43700 47000 44400 47000 4
N 45300 47000 45700 47000 4
N 47200 45800 47500 45800 4
N 47200 44800 47700 44800 4
N 47200 47000 47800 47000 4
N 47200 46000 47500 46000 4
N 47800 46500 47700 46500 4
N 47700 46500 47700 44800 4
N 47500 46000 47500 45800 4
N 45500 47000 45500 48200 4
C 50200 46400 1 0 0 lm7805-1.sym
{
T 51800 47700 5 10 0 0 0 0 1
device=7805
T 51600 47400 5 10 1 1 0 6 1
refdes=U2
T 50200 46400 5 10 1 1 0 0 1
footprint=TO220W
}
N 47500 45900 51000 45900 4
N 51000 45300 51000 46400 4
C 49600 46900 1 270 0 capacitor-2.sym
{
T 50300 46700 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 50100 46700 5 10 1 1 270 0 1
refdes=C1
T 50500 46700 5 10 0 0 270 0 1
symversion=0.1
T 50000 46200 5 10 1 1 0 0 1
value=680u
T 49600 46900 5 10 1 1 0 0 1
footprint=RCY400P
}
C 49600 45800 1 270 0 capacitor-2.sym
{
T 50300 45600 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 50100 45600 5 10 1 1 270 0 1
refdes=C2
T 50500 45600 5 10 0 0 270 0 1
symversion=0.1
T 50000 45100 5 10 1 1 0 0 1
value=680u
T 49600 45800 5 10 1 1 0 0 1
footprint=RCY400P
}
N 49000 47000 50200 47000 4
N 49800 46900 49800 47000 4
N 49000 46500 49100 46500 4
N 49100 46500 49100 44700 4
N 49100 44700 50200 44700 4
N 49800 45800 49800 45900 4
N 49800 46000 49800 45900 4
N 49800 44900 49800 44700 4
C 51400 48100 1 180 0 diode-1.sym
{
T 51000 47500 5 10 0 0 180 0 1
device=DIODE
T 51100 47600 5 10 1 1 180 0 1
refdes=D1
T 50600 48200 5 10 1 1 0 0 1
value=1N4004
T 51400 48100 5 10 1 1 0 0 1
footprint=diode-wide-leads
}
C 52900 46000 1 90 0 capacitor-1.sym
{
T 52200 46200 5 10 0 0 90 0 1
device=CAPACITOR
T 52400 46200 5 10 1 1 90 0 1
refdes=C3
T 52000 46200 5 10 0 0 90 0 1
symversion=0.1
T 52800 46200 5 10 1 1 0 0 1
value=100n
T 52900 46000 5 10 1 1 0 0 1
footprint=CK06_type_capacitor
}
C 52900 44900 1 90 0 capacitor-1.sym
{
T 52200 45100 5 10 0 0 90 0 1
device=CAPACITOR
T 52400 45100 5 10 1 1 90 0 1
refdes=C4
T 52000 45100 5 10 0 0 90 0 1
symversion=0.1
T 52800 45100 5 10 1 1 0 0 1
value=100n
T 52900 44900 5 10 1 1 0 0 1
footprint=CK06_type_capacitor
}
N 52700 46000 52700 45800 4
N 51000 45900 52700 45900 4
N 52700 44900 52700 44700 4
N 52700 47000 52700 46900 4
N 51400 47900 52000 47900 4
N 52000 47900 52000 47000 4
N 50500 47900 50000 47900 4
N 50000 47900 50000 47000 4
N 51500 43600 52000 43600 4
N 52000 43600 52000 44700 4
N 50600 43600 50000 43600 4
N 50000 43600 50000 44700 4
N 53700 47000 53700 46900 4
N 53700 44700 53700 44800 4
N 52700 45900 53700 45900 4
N 53700 45700 53700 46000 4
N 43700 44200 48400 44200 4
C 54200 45500 1 0 0 gnd-1.sym
C 53800 47100 1 0 0 5V-plus-1.sym
C 54200 44600 1 180 0 5V-minus-1.sym
N 51800 44700 54000 44700 4
N 54000 44700 54000 44600 4
N 51800 47000 54000 47000 4
N 54000 47000 54000 47100 4
N 48400 44200 48400 45900 4
N 53700 45900 54300 45900 4
N 54300 45900 54300 45800 4
C 50200 45300 1 180 1 lm7905-1.sym
{
T 50500 44150 5 10 0 0 180 6 1
device=7905
T 51600 44300 5 10 1 1 180 0 1
refdes=U3
T 50500 43550 5 10 0 0 180 6 1
footprint=TO220W
T 50500 43950 5 10 0 0 180 6 1
symversion=1.0
}
C 46000 48100 1 0 0 output-1.sym
{
T 46100 48400 5 10 0 0 0 0 1
device=OUTPUT
T 45700 47900 5 10 1 1 0 0 1
netname=AC_HOT
}
C 45100 44500 1 0 0 output-1.sym
{
T 45200 44800 5 10 0 0 0 0 1
device=OUTPUT
T 44800 44300 5 10 1 1 0 0 1
netname=AC_NEUT
}
N 46000 48200 45500 48200 4
N 45100 44600 44900 44600 4
N 44900 44600 44900 46000 4
C 45000 43600 1 0 0 output-1.sym
{
T 45100 43900 5 10 0 0 0 0 1
device=OUTPUT
T 44700 43400 5 10 1 1 0 0 1
netname=AC_GND
}
N 45000 43700 44700 43700 4
N 44700 43700 44700 44200 4
C 50600 43400 1 0 0 diode-1.sym
{
T 51000 44000 5 10 0 0 0 0 1
device=DIODE
T 50900 43900 5 10 1 1 0 0 1
refdes=D2
T 51400 43300 5 10 1 1 180 0 1
value=1N4004
T 50600 43400 5 10 1 1 0 0 1
footprint=diode-wide-leads
}
C 53900 44800 1 90 0 diode-1.sym
{
T 53300 45200 5 10 0 0 90 0 1
device=DIODE
T 53400 45100 5 10 1 1 90 0 1
refdes=D4
T 54000 45600 5 10 1 1 270 0 1
value=1N4004
T 53900 44800 5 10 1 1 0 0 1
footprint=diode-wide-leads
}
C 53900 46000 1 90 0 diode-1.sym
{
T 53300 46400 5 10 0 0 90 0 1
device=DIODE
T 53400 46300 5 10 1 1 90 0 1
refdes=D3
T 54000 46800 5 10 1 1 270 0 1
value=1N4004
T 53900 46000 5 10 1 1 0 0 1
footprint=diode-wide-leads
}
T 50000 40700 9 10 1 0 0 0 1
ProxLamp High Voltage Board
T 50000 40400 9 10 1 0 0 0 1
highvoltage-1.sch
T 50000 40100 9 10 1 0 0 0 1
1
T 51500 40100 9 10 1 0 0 0 1
2
T 54000 40100 9 10 1 0 0 0 1
Spencer Russell
T 54000 40400 9 10 1 0 0 0 1
01
