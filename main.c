# Import modules
from __future__ import division
import matplotlib.pyplot as pplt;
                            from matplotlib.pyplot import cm
        import neuron as nrn;
                         from neuron import h
                         import numpy as np
                         import pickle as pkl
                         import pylab as plab
                         import string as str
                         import numbers as nbrs

# Load Hoc Object
                         h = nrn.h

# Temperature
        h.celsius = 37


# Define cell class and sections
class cell:
def __init__(self, name='neuron', soma=None, axon=None, apic=None, basl=None):
self.soma = soma if soma is not None else []
self.axon = axon if axon is not None else []
self.apic = apic if apic is not None else []
self.basl = basl if basl is not None else []
self.all = self.soma + self.axon + self.apic + self.basl

def delete(self):  # Redundant notation for creating empty cell subsets
self.soma = None
self.axon = None
self.apic = None
self.basl = None
self.all = None

def __str__(self):  # Returns the string representation of self
return self.name

# I have questions about these helper / getter functions,
        def __getitem__(self, soma):  # Get cell subsets
return getattr(self, 'soma')

def __getitem__(self, axon):
return getattr(self, 'axon')

def __getitem__(self, apic):
return getattr(self, 'apic')

def __getitem__(self, basl):
return getattr(self, 'basl')

def __call__(self, soma):
self.soma = soma


# v_vec.record(soma(0.5)._ref_v)


# t_vec.record(h._ref_t)

def load(filename, fileformat=None, cell=None, use_axon=True, xshift=0, yshift=0, zshift=0):
if cell is None:
        cell = cell(name=str.join(filename.split('.')[:-1]))
if fileformat is None:
        fileformat = filename.split('.')[-1]
name_form = {1: 'soma[%d]', 2: 'axon[%d]', 3: 'apic[%d]', 4: 'basl[%d]'}

# Load NeuroMorpho file
h.load_file('import3d.hoc')
cell = h.Import3d_SWC_read()
cell.input('control_9_wt.CNG.swc')
i3d = h.Import3d_GUI(cell, 0)
i3d.instantiate(None)

# Make a list of the swc section objects
swc_secs = i3d.swc.sections;
print swc_secs.count()
swc_secs = [swc_secs.object(i) for i in xrange(int(swc_secs.count()))]

# Initialize the lists of sections
sec_list = {1: cell.soma, 2: cell.axon, 3: cell.apic, 4: cell.basl}

# Name and create the sections
real_secs = {}
for swc_sec in swc_secs:
        cell_part = int(swc_sec.type)

# Skip everything else if it's an axon and we're not supposed to use it... or if is_subsidiary
if (not (use_axon) and cell_part == 2) or swc_sec.is_subsidiary:
continue

# Figure out the name of the new section
if cell_part not in name_form:
raise Exception('unsupported point type')
name = name_form[cell_part] % len(sec_list[cell_part])

# Connect to parent, if needed
if swc_sec.parentsec is not None:
sec.connect(real_secs[swc_sec.parentsec.hname()](swc_sec.parentx))

# Define shape
if swc_sec.first == 1:
h.pt3dstyle(1, swc_sec.raw.getval(0, 0), swc_sec.raw.getval(1, 0),
swc_sec.raw.getval(2, 0), sec=sec)

j = swc_sec.first
xx, yy, zz = [swc_sec.raw.getrow(i).c(j) for i in xrange(3)]
dd = swc_sec.d.c(j)
# This never happens in SWC files, but can happen in other formats supported by NEURON's Import3D GUI
if swc_sec.iscontour_:
        raise Exception('Unsupported section style: contour')
# Single point soma; treat as sphere
if dd.size() == 1:
x, y, z, d = [dim.x[0] for dim in [xx, yy, zz, dd]]
for xprime in [x - d / 2., x, x + d / 2.]:
h.pt3dadd(xprime + xshift, y + yshift, z + zshift, d, sec=sec)
else:
for x, y, z, d in zip(xx, yy, zz, dd):
h.pt3dadd(x + xshift, y + yshift, z + zshift, d, sec=sec)
# Store the section in the appropriate list in the cell and lookup table
sec_list[cell_part].append(sec)
real_secs[swc_sec.hname()] = sec
        cell.all = cell.soma + cell.apic + cell.dend + cell.axon
return cell


# Insert axial resistance and capacitance
for sec in h.allsec():
        sec.Ra = 160  # Axial resistance in Ohm*cm
        sec.cm = 1  # Membrane capacitance in uF/cm^2

# Insert passive and active mechanisms in soma
cell('soma').g_pas = 0.001;
cell('soma').e_pas = -60  # Passive conductance in S/cm2; Leak reversal potential mV
cell('soma').gl_hh = 0.0002;
cell('soma').el_hh = -60;
cell('soma').gnabar_hh = 0.12;
cell('soma').ena = 40;
cell('soma').gkbar_hh = 0.036;
cell('soma').ek = -90
# Insert passive and active mechanisms in axon
cell('axon').g_pas = 0.001;
cell('axon').e_pas = -60
cell('axon').gl_hh = 0.0002;
cell('axon').el_hh = -60;
cell('axon').gnabar_hh = 0.12;
cell('axon').gkbar_hh = 0.036
# Insert passive and active mechanisms in apical
cell('apic').g_pas = 0.001;
cell('apic').e_pas = -60
cell('apic').gl_hh = 0.0002;
cell('apic').el_hh = -60;
cell('apic').gnabar_hh = 0.12;
cell('apic').ena = 40;
cell('apic').gkbar_hh = 0.036;
cell('apic').ek = -90
# Insert passive and active mechanisms in basal
cell('basl').g_pas = 0.001;
cell('basl').e_pas = -60
cell('basl').gl_hh = 0.0002;
cell('basl').el_hh = -60;
cell('basl').gnabar_hh = 0.012;
cell('basl').ena = 40;
cell('basl').gkbar_hh = 0.0036;
cell('basl').ek = -90

# Header for values to test
syn_number = 1  # Number of synapses to create

# Create one point process (i.e. one synapse)
# syn = h.AlphaSynapse(0.5, cell('apic'))
# syn.onset=5
# syn.tau=5
# syn.gmax=1.5
# syn.e=0

# Create multiple point processes (i.e. multiple synapses)
        syn_list_apic = []
for i in range(0, syn_number):
# temp = h.AlphaSynapse(0.5, cell('apic'))           # Location
temp = h.AlphaSynapse(0.5)
temp.onset = 5 + (i * 2)  # Onset - i.e. delay (ms)
temp.tau = 5  # Tau (ms)
temp.gmax = 1.5  # gmax (umho)
temp.e = 0  # e (mV)
syn_list_apic.append(temp)

# Keep track of point processes
syn_vectors = []  # storage for individual synapses
for j in syn_list_apic:
        temp = h.Vector()
temp.record(j._ref_i)
syn_vectors.append(temp)

# Set recording variables
v_vec = h.Vector()  # Membrane potential vector
        t_vec = h.Vector()  # Time stamp vector
# v_vec.record(h.ref(cell('soma')(0.5).v_vec.record))
# v_vec.record(cell('soma')(0.5)._ref_v)
        so = cell('soma')
v_vec.record(so(0.5)._ref_v)  # MSB Attempt to change the syntax
t_vec.record(h._ref_t)

# v_vec.record(soma(0.5)._ref_v)
# t_vec.record(h._ref_t)

# Run the simulation
h.tstop = 40
h.run()
h.load_file('stdrun.hoc')

# Plot the results
pplt.figure(figsize=(10, 5))  # Default figsize is (8,6)
pplt.plot(t_vec, v_vec)
pplt.xlabel('Time (ms)')
pplt.ylabel('Membrane potential (mV)')
pplt.show()

# Save the results
with open('t_vec.p', 'w') as t_vec_file:
pkl.dump(t_vec.to_python(), t_vec_file)
with open('v_vec.p', 'w') as v_vec_file:
pkl.dump(v_vec.to_python(), v_vec_file)