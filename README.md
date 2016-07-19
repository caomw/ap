# The Aggregated Partition for Discovering Objects

#### INTRODUCTION:

The Aggregated Partition (AP) and the Selective AP are two models for the problem of unsupervised object discovery in image collections.

This is a repository with an implementation of the models described in our [CVPR2016 Workshop paper](http://www.cv-foundation.org/openaccess/content_cvpr_2016_workshops/w24/html/Lopez-Sastre_Unsupervised_Robust_Feature-Based_CVPR_2016_paper.html). We provide here the codes and data needed to reproduce all the experiments detailed in the paper.

### License

AP is released under the MIT License (refer to the LICENSE file for details).

#### CITING

If you make use of this data and software, please cite the following reference in any publications:


	@inproceedings{LopezSastre2016,
        Title                    = {Unsupervised Robust Feature-based Partition Ensembling to Discover Categories},
        Author                   = {Lopez-Sastre, R.~J.},
        Booktitle                = {CVPR Workshops},
        Year                     = {2016}
	}


#### REQUIREMENTS:

The AP code is developed and tested under Ubuntu 14.04. Matlab is required.

#### COMPILING:

Before running any experiment, you should follow the following instructions:

##### Mex files
Open Matlab and compile the following two mex files:

```Shell
    cwd=cd; %local path where you have installed AP release
    cd code/generic_functions/
    mex chi_squared_c.c
    cd(cwd);
    cd code/code_weights/
    mex compute_distances_c.c
    cd(cwd);
```
##### Clustering Aggregation THIRD-PARTY SOFTWARE


```Shell
    cd code/aggregation/ca_code
    make all   
```

---

#### DISCOVERING CATEGORIES:

We provide in the folder "experiments" the corresponding Matlab scripts
to use the AP models in the three datasets described in our paper.

For example, to discover categories in the Caltech-256, simply, open Matlab
and go to the corresponding folder and run the provided script.

+ For the AP model:

```Shell
   cd experiments/Caltech-256/SpectralClustering/AP
   experiment_ap   
```

+ For the Selective AP:
```Shell
   cd experiments/Caltech-256/SpectralClustering/SAP
   experiment_selective_ap 
```
---
