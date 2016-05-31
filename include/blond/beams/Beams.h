/*
 * Beams.h
 *
 *  Created on: Mar 9, 2016
 *      Author: kiliakis
 */

#ifndef BEAMS_BEAMS_H_
#define BEAMS_BEAMS_H_

#include <blond/configuration.h>

class Beams {
public:
   ftype *dt;
   ftype *dE;
   ftype mean_dt;
   ftype mean_dE;
   ftype sigma_dt;
   ftype sigma_dE;
   ftype ratio;
   ftype epsn_rms_l;
   int n_macroparticles_lost;
   int n_macroparticles;
   long intensity;
   //GeneralParameters *gp;
   int *id;
   Beams(const int _n_macroparticles,const long _intensity);
   ~Beams();
   int n_macroparticles_alive();
   void losses_longitudinal_cut(const ftype *__restrict dt,
                                const ftype dt_min, const ftype dt_max, int *__restrict id);
   void losses_energy_cut(const ftype *__restrict dE, const ftype dE_min,
                          const ftype dE_max, int *__restrict id);

   void statistics();
};

#endif /* BEAMS_BEAMS_H_ */