The Lesion Sizing Toolkit (LSTK)
================================

Overview
--------

This is a module for the `Insight Toolkit (ITK) <http://itk.org>`_ that
provides a generic, modular, and extensible architecture for lesion sizing
algorithms in medical images as well as a reference algorithm for lung
solid lesion segmentation in CT images.

For more information, see the `Insight Journal article <http://hdl.handle.net/10380/3369>`_::

  Liu X., Helba B., Krishnan K., Reynolds P., McCormick M., Turner W., Ibáñez L., Yankelevitz D., Avila R.
  Fostering Open Science in Lung Cancer Lesion Sizing with ITK module LSTK
  The Insight Journal. January-December. 2012.
  http://hdl.handle.net/10380/3369
  http://www.insight-journal.org/browse/publication/869

Data
----
The project has extensively used the CT lesion data assembled by NIST for the
`Biochange 2008 Pilot Study <https://www.itl.nist.gov/iad/894.05/biochange2008/Biochange2008-webpage.htm>`_.
The data collection can be obtained via ftp from
`here <ftp://ftp.nist.gov/pub/itl/biochange/Biochange2008/FindingCT_ScansForBiochange2008.htm>`_.

The team participated in the
`Volcano'09 Challenge benchmark <http://www.via.cornell.edu/challenge/>`_.

The project has also been used on the
`BioChange 2011 <ftp://ftp.nist.gov/pub/itl/biochange/BiochangeChallenge/BiochangeChallengeProtocol.pdf>`_
study. The clinical data used here was chosen from the publicly available
Reference Image Database to Evaluate Therapy Response
(`RIDER <https://wiki.nci.nih.gov/display/CIP/RIDER>`_) database and from the
NIST-generated CT phantom series. 96 pairs of clinical
`datasets <https://www.nist.gov/itl/iad/dmg/biochangechallenge.cfm>`_ are
present. Datasets have slice thickness varying from 0.63 to 2.5 mm. Kitware's
results on the Biochange2011 challenge may be found
`here <https://public.kitware.com/LesionSizingKit/index.php/Users/BioChange2011Results>`_.

License
-------

This software is distributed under the Apache 2.0 license. Please see
the *LICENSE* file for details.

Acknowledgements
----------------

This work was supported by the Optical Society of America (OSA), the Air Force
Research Laboratory (AFRL), and the National Library of Medicine (NLM).
