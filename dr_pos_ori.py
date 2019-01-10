#! /usr/bin/env python

# from ROOT import RooFit as rf
# from ROOT import *
import ROOT as r
import sys
import os
# import glob
import math
# import datetime
# import numpy as np
# from array import array
# import re
# import matplotlib as ml
# import matplotlib.pyplot as plt
# from PIL import Image

# R.gSystem.Load('libRooFit')


def read_tubs():
    finname = 'beamtest2017B-tubs.tsv'
    fin = open( finname )
    out = []
    for line in fin:
        if len( line ) < 3:
            continue
        if '#' == line[0]:
            continue
        lin = line.rstrip( '\n' ).rstrip( '\r' ).split( '\t' )
        if lin[3] == '':
            continue
        x = {}
        try:
            x['name'] = lin[9]
            x['rin'] = float( lin[1] )
            x['rout'] = float( lin[2] )
            x['length'] = float( lin[4] )
            x['material'] = lin[5]
            x['zcentre'] = float( lin[6] )
            x['zupstream'] = float( lin[7] )
            x['zdownstream'] = float( lin[8] )
            out.append( x )
        except ValueError:
            print 'ERROR'
            print lin
    return out


def getCanvas( name, cw=600, ch=600, margins=-1 ):
    c1 = r.TCanvas( name, name, cw, ch )
    if not '-b' in sys.argv:
        c1.SetWindowSize(cw + (cw - c1.GetWw()), ch + (ch - c1.GetWh()));
    else:
        c1.SetCanvasSize(cw, ch)
    if margins != -1:
        c1.SetLeftMargin( margins[0] )
        c1.SetBottomMargin( margins[1] )
        c1.SetRightMargin( margins[2] )
        c1.SetTopMargin( margins[3] )
    return c1


def main():
    tubs = read_tubs()
    Fin = r.TFile( 'build/Simulation.root' )
    t = Fin.Get( 'PC' )
    # c1 = getCanvas( 'c1', 9*120, 9*110, [.1/(12./11), .1, .1/(12./11), .1] )
    c1 = getCanvas( 'c1', 9*120, 9*90, [.1/(12./9), .1, .1/(12./9), .1] )
    cut = ''
    sel = 'OriginX:OriginY'
    t.Draw( '{sel}>>h(120,-250,350,90,-250,200)'.format( sel=sel ), cut )
    cut = 'PC1Edep>200 && PC2Edep>200 && PC3Edep>200'
    t.SetMarkerColor( r.kGreen )
    t.Draw( sel, cut, 'same' )
    line = r.TLine()
    line.SetLineColor( r.kRed )
    line.SetLineWidth( 2 )
    z0 = 1136.7+34.55+9.2+5.2
    k = 1
    for tub in tubs:
        rin = tub['rin']
        rout = tub['rout']
        length=tub['length']
        z = tub['zcentre']
        # Top
        line.DrawLine( k*(z-z0-length/2.), rin*k, k*(z-z0-length/2.), rout*k )
        line.DrawLine( k*(z-z0-length/2.), rout*k, k*(z-z0+length/2.), rout*k )
        line.DrawLine( k*(z-z0+length/2.), rin*k, k*(z-z0+length/2.), rout*k )
        line.DrawLine( k*(z-z0-length/2.), rin*k, k*(z-z0+length/2.), rin*k )
        # Bottom
        line.DrawLine( k*(z-z0-length/2.), -rin*k, k*(z-z0-length/2.), -rout*k )
        line.DrawLine( k*(z-z0-length/2.), -rout*k, k*(z-z0+length/2.), -rout*k )
        line.DrawLine( k*(z-z0+length/2.), -rin*k, k*(z-z0+length/2.), -rout*k )
        line.DrawLine( k*(z-z0-length/2.), -rin*k, k*(z-z0+length/2.), -rin*k )
    lat = r.TLatex()
    lat.SetTextFont( 12 )
    lat.SetTextSize( .03 )
    lat.DrawLatexNDC( .2, .92, 'Positron origin, 10 million e^{+} are simulated' )
    lat.DrawLatexNDC( .32, .31, 'All positrons' )
    lat.SetTextColor( r.kGreen+1 )
    lat.DrawLatexNDC( .32, .28, r'Positrons which hit all three positron counters with E_{dep, i}>200 keV' )
    lat.SetTextColor( r.kRed )
    lat.DrawLatexNDC( .32, .25, r'Beam pipe' )
    lat.SetTextColor( r.kBlack )
    lat.DrawLatexNDC( .6, .025, 'Beam/horizontal direction, mm' )
    lat.SetTextAngle( 90 )
    lat.DrawLatexNDC( .025, .6, 'Vertical direction, mm' )
    c1.Update()
    #
    c2 = getCanvas( 'c2' )
    r.gStyle.SetOptStat( 0 )
    t.SetLineWidth( 2 )
    t.SetLineColor( r.kGreen+2 )
    t.Draw( 'PC2Edep >> h2(100,0,3000)', 'PC2Edep>0' )
    t.SetLineColor( r.kRed )
    t.Draw( 'PC1Edep', 'PC1Edep>0', 'same' )
    t.SetLineColor( r.kBlue )
    t.Draw( 'PC3Edep', 'PC3Edep>0', 'same' )
    lat.SetTextColor( r.kBlack )
    lat.SetTextSize( .04 )
    lat.SetTextAngle( 0 )
    lat.DrawLatexNDC( .8, .025, r'E_{dep}, keV' )
    lat.SetTextAngle( 90 )
    lat.DrawLatexNDC( .0225, .8, 'Entries' )
    lat.SetTextAngle( 0 )
    lat.SetTextColor( r.kRed )
    lat.DrawLatexNDC( .7, .85, 'PC1, small' )
    lat.SetTextColor( r.kGreen+2 )
    lat.DrawLatexNDC( .7, .80, 'PC2, thick' )
    lat.SetTextColor( r.kBlue )
    lat.DrawLatexNDC( .7, .75, 'PC3, thin' )
    c2.Update()
    #
    c3 = getCanvas( 'c3' )
    r.gStyle.SetOptStat( 0 )
    t.SetLineWidth( 2 )
    t.SetLineColor( r.kGreen+2 )
    t.Draw( 'PC2Time >> h3(100,0,15000)', 'PC2Edep>0' )
    t.SetLineColor( r.kRed )
    t.Draw( 'PC1Time', 'PC1Edep>0', 'same' )
    t.SetLineColor( r.kBlue )
    t.Draw( 'PC3Time', 'PC3Edep>0', 'same' )
    lat.SetTextColor( r.kBlack )
    lat.SetTextSize( .04 )
    lat.SetTextAngle( 0 )
    lat.DrawLatexNDC( .8, .025, r'Time, ns' )
    lat.SetTextAngle( 90 )
    lat.DrawLatexNDC( .0225, .8, 'Entries' )
    lat.SetTextAngle( 0 )
    lat.SetTextColor( r.kRed )
    lat.DrawLatexNDC( .7, .85, 'PC1, small' )
    lat.SetTextColor( r.kGreen+2 )
    lat.DrawLatexNDC( .7, .80, 'PC2, thick' )
    lat.SetTextColor( r.kBlue )
    lat.DrawLatexNDC( .7, .75, 'PC3, thin' )
    c3.Update()
    #
    raw_input( 'Press ENTER to continue, please.' )
    if 'save' in sys.argv:
        c1.SaveAs( 'originx_originy.png' )
        c2.SaveAs( 'edep.png' )
        c3.SaveAs( 'time.png' )


if __name__ == '__main__':
    main()
