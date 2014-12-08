
#include <stdlib.h>

#include <gsl/gsl_math.h>

#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>

#include <gsl/gsl_sf_gamma.h>

struct monte_int{
	double volume;
	double est_error;
};
struct monte_params{
	int n;
};

double shape(double *k, size_t dim, void *params){
	double total = 0.;
	int i;
	struct monte_params * mp = (struct monte_params *)params;
	for(i = 0; i < 5; i++){
		total += pow(k[i], mp->n);
	}
	if(total <= 1.0){
		return 1;
	}else{
		return 0.0;
	}
}

struct monte_int volume(int exponent, size_t num_samples){
	double result, error_estimate;
	double lower_bounds[5] = {0., 0., 0., 0., 0.};
	double upper_bounds[5] = {1., 1., 1., 1., 1.};
	gsl_rng *r;
	unsigned long seed = 1UL;
	struct monte_params params = {exponent};
	gsl_monte_function G = { &shape, 5, &params};


	r = gsl_rng_alloc(gsl_rng_taus2);
	gsl_rng_set(r, seed);
	

	gsl_monte_miser_state *sm = gsl_monte_miser_alloc (5);

	gsl_monte_miser_integrate (&G, lower_bounds, upper_bounds, 5, num_samples, r, sm, &result, &error_estimate);
	gsl_monte_miser_free (sm);
	struct monte_int results;
	results.volume = pow(2.0, 5)*result;
	results.est_error = error_estimate;
	gsl_rng_free(r);
	return results;
}

int main(void){
	int n;
	size_t num_samples = 10000;

	struct monte_int results;
	FILE *f = fopen("results.txt", "w");
	if(f == NULL){
		printf("error opening file!\n");
		exit(1);
	}
	for(n = 1; n < 10; n++){ 
		do{
			results = volume(n, num_samples);
			num_samples += 500;
		}while(results.est_error >= 0.03);
		fprintf(f, "%d \t %.6f \n", n, results.volume); 
		printf("volume of %.6f for n = %d, with %zu samples, and error = %.6f \n", results.volume, n, num_samples, results.est_error);
	}
	fclose(f);
}