#include <blond/beams/Distributions.h>
#include <blond/constants.h>
#include <blond/input_parameters/GeneralParameters.h>
#include <blond/math_functions.h>
#include <blond/trackers/Tracker.h>
#include <blond/utilities.h>
#include <gtest/gtest.h>

using namespace std;


void test_vector(const f_vector_t &refV, const f_vector_t &realV,
                 const string &varName, const double epsilon)
{
    ASSERT_EQ(refV.size(), realV.size());
    for (uint i = 0; i < refV.size(); ++i) {
        auto ref = refV[i];
        auto real = realV[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of " << varName << " failed on i " << i << "\n";
    }
}

class testDistributions : public ::testing::Test {

protected:
    // Bunch parameters
    const long long N_b = 0; // Intensity

    // Machine and RF parameters
    const double tau_0 = 0.4e-9;
    const double radius = 25;
    const double C = 2 * constant::pi * radius; // Machine circumference [m]
    const double p_i = 310891054.809;           // Synchronous momentum [eV/c]
    const double h = 1;                          // Harmonic number
    const double V = 8000;                      // RF voltage [V]
    const double dphi = -constant::pi;          // Phase modulation/offset
    const double gamma_t = 4.076750841;         // Transition gamma
    const double alpha =
        1.0 / gamma_t / gamma_t; // First order mom. comp. factor
    const int alpha_order = 1;
    const int n_sections = 1;
    // Tracking details

    int N_t = 1000;  // Number of turns to track
    int N_p = 1000; // Macro-particles

    int N_slices = 100; // = (2^8)

    RfParameters *RfP1, *RfP2;
    RingAndRfSection *long_tracker1, *long_tracker2;


    virtual void SetUp()
    {
        omp_set_num_threads(1);

        f_vector_2d_t momentumVec(n_sections, f_vector_t(N_t + 1, p_i));

        f_vector_2d_t alphaVec(n_sections, f_vector_t(alpha_order + 1, alpha));

        f_vector_t CVec(n_sections, C);

        f_vector_2d_t hVec(n_sections, f_vector_t(N_t + 1, h));

        f_vector_2d_t voltageVec(n_sections, f_vector_t(N_t + 1, V));

        f_vector_2d_t dphiVec(n_sections, f_vector_t(N_t + 1, dphi));

        Context::GP = new GeneralParameters(N_t, CVec, alphaVec,
                                            alpha_order, momentumVec,
                                            GeneralParameters::particle_t::proton);


        auto GP = Context::GP;
        auto Beam = Context::Beam = new Beams(GP, N_p, N_b);

        auto RfP = Context::RfP = new RfParameters(GP, n_sections, hVec,
                voltageVec, dphiVec);

    }

    virtual void TearDown()
    {
        // Code here will be called immediately after each test
        // (right before the destructor).
        delete Context::GP;
        delete Context::Beam;
        delete Context::RfP;
    }
};

TEST(testHelpers, line_density_function1)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/line_density_function1/");

    int size = 100;
    f_vector_t coord_array(size);
    for (int i = 0; i < size; i++) {
        coord_array[i] = cos(i);
    }

    string dist_type = "waterbag";
    double bunch_length = 1.5;
    auto ret = line_density_function(coord_array, dist_type, bunch_length);

    f_vector_t v;
    util::read_vector_from_file(v, params + "density.txt");

    ASSERT_EQ(v.size(), ret.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = ret[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of density failed on i " << i << endl;
    }


}


TEST(testHelpers, line_density_function2)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/line_density_function2/");
    int size = 100;

    f_vector_t coord_array(size);
    for (int i = 0; i < size; i++) {
        coord_array[i] = cos(i);
    }

    string dist_type = "binomial";
    double bunch_length = 1.;
    auto ret = line_density_function(coord_array, dist_type, bunch_length,
                                     0.5, 0.6);
    f_vector_t v;
    util::read_vector_from_file(v, params + "density.txt");

    ASSERT_EQ(v.size(), ret.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = ret[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of density failed on i " << i << endl;
    }

}

TEST(testHelpers, line_density_function3)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/line_density_function3/");
    int size = 100;

    f_vector_t coord_array(size);
    for (int i = 0; i < size; i++) {
        coord_array[i] = cos(i);
    }

    string dist_type = "parabolic_line";
    double bunch_length = 2.;
    auto ret = line_density_function(coord_array, dist_type, bunch_length);
    f_vector_t v;
    util::read_vector_from_file(v, params + "density.txt");

    ASSERT_EQ(v.size(), ret.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = ret[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of density failed on i " << i << endl;
    }

}

TEST(testHelpers, line_density_function4)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/line_density_function4/");
    int size = 100;

    f_vector_t coord_array(size);
    for (int i = 0; i < size; i++) {
        coord_array[i] = cos(i);
    }

    string dist_type = "gaussian";
    double bunch_length = 1.2;
    auto ret = line_density_function(coord_array, dist_type, bunch_length, 1.);
    f_vector_t v;
    util::read_vector_from_file(v, params + "density.txt");

    ASSERT_EQ(v.size(), ret.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = ret[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of density failed on i " << i << endl;
    }

}


TEST(testHelpers, line_density_function5)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/line_density_function5/");
    int size = 100;

    f_vector_t coord_array(size);
    for (int i = 0; i < size; i++) {
        coord_array[i] = cos(i);
    }

    string dist_type = "cosine_squared";
    double bunch_length = 1.2;
    auto ret = line_density_function(coord_array, dist_type, bunch_length, 1.);
    f_vector_t v;
    util::read_vector_from_file(v, params + "density.txt");

    ASSERT_EQ(v.size(), ret.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = ret[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of density failed on i " << i << endl;
    }

}

TEST(testHelpers, line_density_function_deathtest1)
{
    f_vector_t x;
    ASSERT_DEATH(line_density_function(x, "blabla", 0.),
                 "[line_density_function]\\s*");
}


TEST(testHelpers, distribution_density_function_deathtest1)
{
    f_vector_t x;
    ASSERT_DEATH(distribution_density_function(x, "blabla", 0.),
                 "[distribution_density_function]\\s*");
}


TEST(testHelpers, distribution_density_function1)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/distribution_density_function1/");
    int size = 100;

    f_vector_t coord_array(size);
    for (int i = 0; i < size; i++) {
        coord_array[i] = cos(i);
    }

    string dist_type = "binomial";
    double bunch_length = 2.;
    auto ret = distribution_density_function(coord_array, dist_type, bunch_length, 0.7);
    f_vector_t v;
    util::read_vector_from_file(v, params + "density.txt");

    ASSERT_EQ(v.size(), ret.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = ret[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of density failed on i " << i << endl;
    }

}


TEST(testHelpers, distribution_density_function2)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/distribution_density_function2/");
    int size = 100;

    f_vector_t coord_array(size);
    for (int i = 0; i < size; i++) {
        coord_array[i] = cos(i);
    }

    string dist_type = "waterbag";
    double bunch_length = 2.;
    auto ret = distribution_density_function(coord_array, dist_type, bunch_length);
    f_vector_t v;
    util::read_vector_from_file(v, params + "density.txt");

    ASSERT_EQ(v.size(), ret.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = ret[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of density failed on i " << i << endl;
    }

}

TEST(testHelpers, distribution_density_function3)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/distribution_density_function3/");
    int size = 100;

    f_vector_t coord_array(size);
    for (int i = 0; i < size; i++) {
        coord_array[i] = cos(i);
    }

    string dist_type = "parabolic_line";
    double bunch_length = 2.;
    auto ret = distribution_density_function(coord_array, dist_type, bunch_length, 1.1);
    f_vector_t v;
    util::read_vector_from_file(v, params + "density.txt");

    ASSERT_EQ(v.size(), ret.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = ret[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of density failed on i " << i << endl;
    }

}


TEST(testHelpers, distribution_density_function4)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/distribution_density_function4/");
    int size = 100;

    f_vector_t coord_array(size);
    for (int i = 0; i < size; i++) {
        coord_array[i] = cos(i);
    }

    string dist_type = "gaussian";
    double bunch_length = 3.;
    auto ret = distribution_density_function(coord_array, dist_type, bunch_length);
    f_vector_t v;
    util::read_vector_from_file(v, params + "density.txt");

    ASSERT_EQ(v.size(), ret.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = ret[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of density failed on i " << i << endl;
    }

}


TEST(testHelpers, minmax_location1)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/minmax_location1/");
    int size = 100;
    f_vector_t x = mymath::linspace(0.0, size, size);
    f_vector_t f(size);

    for (int i = 0; i < size; i++) f[i] = cos(i);
    f_vector_t min_x_position, max_x_position;
    f_vector_t min_values, max_values;

    minmax_location(x, f, min_x_position, max_x_position,
                    min_values, max_values);
    f_vector_t v;

    util::read_vector_from_file(v, params + "min_x_position.txt");
    test_vector(v, min_x_position, "min_x_position", epsilon);

    util::read_vector_from_file(v, params + "max_x_position.txt");
    test_vector(v, max_x_position, "max_x_position", epsilon);

    util::read_vector_from_file(v, params + "min_values.txt");
    test_vector(v, min_values, "min_values", epsilon);

    util::read_vector_from_file(v, params + "max_values.txt");
    test_vector(v, max_values, "max_values", epsilon);

}


TEST(testHelpers, minmax_location2)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/minmax_location2/");
    int size = 200;
    f_vector_t x = mymath::linspace(0.0, size, size);
    f_vector_t f(size);

    for (int i = 0; i < size; i++) f[i] = (i % 2) ? cos(i) : sin(i);

    f_vector_t min_x_position, max_x_position;
    f_vector_t min_values, max_values;

    minmax_location(x, f, min_x_position, max_x_position,
                    min_values, max_values);
    f_vector_t v;

    util::read_vector_from_file(v, params + "min_x_position.txt");
    test_vector(v, min_x_position, "min_x_position", epsilon);

    util::read_vector_from_file(v, params + "max_x_position.txt");
    test_vector(v, max_x_position, "max_x_position", epsilon);

    util::read_vector_from_file(v, params + "min_values.txt");
    test_vector(v, min_values, "min_values", epsilon);

    util::read_vector_from_file(v, params + "max_values.txt");
    test_vector(v, max_values, "max_values", epsilon);

}



TEST(testHelpers, potential_well_cut1)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/potential_well_cut1/");
    int size = 100;
    f_vector_t theta_coord_array = mymath::linspace(0.0, size, size);
    f_vector_t potential_array(size);

    for (int i = 0; i < size; i++)
        potential_array[i] = (i % 2) ? cos(i) : sin(i);

    f_vector_t theta_coord_sep, potential_well_sep;
    f_vector_t min_values, max_values;

    potential_well_cut(theta_coord_array, potential_array,
                       theta_coord_sep, potential_well_sep);

    f_vector_t v;

    util::read_vector_from_file(v, params + "theta_coord_sep.txt");
    test_vector(v, theta_coord_sep, "theta_coord_sep", epsilon);

    util::read_vector_from_file(v, params + "potential_well_sep.txt");
    test_vector(v, potential_well_sep, "potential_well_sep", epsilon);

}


TEST(testHelpers, potential_well_cut_deathtest1)
{
    int size = 5;
    f_vector_t theta_coord_array = mymath::linspace(0.0, size, size);
    f_vector_t potential_array(size);

    for (int i = 0; i < size; i++)
        potential_array[i] = (i % 2) ? cos(i) : sin(i);

    f_vector_t theta_coord_sep, potential_well_sep;
    f_vector_t min_values, max_values;

    ASSERT_DEATH(potential_well_cut(theta_coord_array, potential_array,
                                    theta_coord_sep, potential_well_sep)
                 , "[potential_well_cut]\\s+");

}


TEST(testHelpers, potential_well_cut_deathtest2)
{
    int size = 10;
    f_vector_t theta_coord_array = mymath::linspace(0.0, size, size);
    f_vector_t potential_array(size);

    for (int i = 0; i < size; i++)
        potential_array[i] = cos(i);

    f_vector_t theta_coord_sep, potential_well_sep;
    f_vector_t min_values, max_values;

    ASSERT_DEATH(potential_well_cut(theta_coord_array, potential_array,
                                    theta_coord_sep, potential_well_sep)
                 , "[potential_well_cut]\\s+");

}


TEST(testHelpers, potential_well_cut2)
{
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/potential_well_cut2/");
    int size = 10;
    f_vector_t theta_coord_array = mymath::linspace(0.0, size, size);
    f_vector_t potential_array(size);

    for (int i = 0; i < size; i++)
        potential_array[i] = sin(i);

    f_vector_t theta_coord_sep, potential_well_sep;
    f_vector_t min_values, max_values;

    potential_well_cut(theta_coord_array, potential_array,
                       theta_coord_sep, potential_well_sep);

    f_vector_t v;

    util::read_vector_from_file(v, params + "theta_coord_sep.txt");
    test_vector(v, theta_coord_sep, "theta_coord_sep", epsilon);

    util::read_vector_from_file(v, params + "potential_well_sep.txt");
    test_vector(v, potential_well_sep, "potential_well_sep", epsilon);

}



TEST(testHelpers, potential_well_cut_deathtest3)
{
    auto epsilon = 1e-8;
    int size = 10;
    f_vector_t theta_coord_array = mymath::linspace(0.0, size, size);
    f_vector_t potential_array(size);

    for (int i = 0; i < size; i++)
        potential_array[i] = exp(i);

    f_vector_t theta_coord_sep, potential_well_sep;
    f_vector_t min_values, max_values;

    ASSERT_DEATH(potential_well_cut(theta_coord_array, potential_array,
                                    theta_coord_sep, potential_well_sep)
                 , "[potential_well_cut]\\s+");
}





// TEST_F(testDistributions, matched_from_line_density1)
// {
//     auto RfP = Context::RfP;
//     auto beam = Context::Beam;
//     auto epsilon = 1e-8;
//     auto params = string(TEST_FILES "/Distributions/line_density1/");

//     longitudinal_bigaussian(tau_0 / 4, 0, -1, false);
//     auto slices = new Slices(RfP, beam, 100);
//     slices->track();

//     auto long_tracker = new RingAndRfSection(RfP);
//     vector<RingAndRfSection *> trackerList{long_tracker};
//     auto fullRing = new FullRingAndRf(trackerList);

//     map<string, string> line_density_opt;
//     line_density_opt["type"] = "gaussian";
//     line_density_opt["exponent"] = "2";
//     line_density_opt["bunch_length"] = "200e-9";
//     line_density_opt["density_variable"] = "density_from_J";

//     f_vector_t R_shunt, f_res, Q_factor;

//     R_shunt.reserve(100);
//     f_res.reserve(100);
//     Q_factor.reserve(100);

//     random_device rd;
//     mt19937 e2(rd());
//     uniform_real_distribution<> dist(0, 1);

//     for (uint i = 0; i < 100; i++) {
//         f_res.push_back(dist(e2) * 1e9);
//         Q_factor.push_back(dist(e2));
//         R_shunt.push_back(dist(e2) * 1e6);
//     }

//     auto resonator = new Resonators(R_shunt, f_res, Q_factor);

//     vector<Intensity *> wakeSourceList({resonator});
//     auto indVoltTime = new InducedVoltageTime(slices, wakeSourceList);
//     vector<InducedVoltage *> indVoltList({indVoltTime});
//     auto totVolt = new TotalInducedVoltage(beam, slices, indVoltList);



//     // NOTE continue here
//     matched_from_line_density(beam, fullRing, line_density_opt,
//                               FullRingAndRf::lowest_freq, totVolt);

//     delete slices;
//     delete totVolt;
//     delete indVoltTime;
//     delete long_tracker;
//     delete fullRing;
// }

TEST_F(testDistributions, matched_from_line_density1)
{
    auto RfP = Context::RfP;
    auto beam = Context::Beam;
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/line_density1/");

    auto long_tracker = new RingAndRfSection(RfP);
    vector<RingAndRfSection *> trackerList{long_tracker};
    auto fullRing = new FullRingAndRf(trackerList);

    map<string, string> line_density_opt;
    line_density_opt["type"] = "gaussian";
    line_density_opt["bunch_length"] = "200e-9";
    line_density_opt["density_variable"] = "density_from_J";

    matched_from_line_density(beam, fullRing, line_density_opt);

    f_vector_t v;
    util::read_vector_from_file(v, params + "dt.txt");

    ASSERT_EQ(v.size(), beam->dt.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = beam->dt[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of beam->dt failed on i " << i << endl;
    }

    util::read_vector_from_file(v, params + "dE.txt");

    ASSERT_EQ(v.size(), beam->dE.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = beam->dE[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of beam->dE failed on i " << i << endl;
    }

    delete long_tracker;
    delete fullRing;
}


TEST_F(testDistributions, matched_from_line_density2)
{
    auto RfP = Context::RfP;
    auto beam = Context::Beam;
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/line_density2/");

    auto long_tracker = new RingAndRfSection(RfP);
    vector<RingAndRfSection *> trackerList{long_tracker, long_tracker};
    auto fullRing = new FullRingAndRf(trackerList);

    map<string, string> line_density_opt;
    line_density_opt["type"] = "binomial";
    line_density_opt["bunch_length"] = "100e-9";
    line_density_opt["density_variable"] = "density_from_J";
    line_density_opt["exponent"] = "1.5";

    matched_from_line_density(beam, fullRing, line_density_opt);

    f_vector_t v;
    util::read_vector_from_file(v, params + "dt.txt");

    ASSERT_EQ(v.size(), beam->dt.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = beam->dt[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of beam->dt failed on i " << i << endl;
    }

    util::read_vector_from_file(v, params + "dE.txt");

    ASSERT_EQ(v.size(), beam->dE.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = beam->dE[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of beam->dE failed on i " << i << endl;
    }

    delete long_tracker;
    delete fullRing;
}


TEST_F(testDistributions, matched_from_distribution_density1)
{
    auto RfP = Context::RfP;
    auto beam = Context::Beam;
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/distribution_density1/");

    auto long_tracker = new RingAndRfSection(RfP);
    vector<RingAndRfSection *> trackerList{long_tracker};
    auto fullRing = new FullRingAndRf(trackerList);

    map<string, string> distribution_density_opt;
    distribution_density_opt["type"] = "binomial";
    distribution_density_opt["bunch_length"] = "100e-9";
    distribution_density_opt["density_variable"] = "density_from_J";
    distribution_density_opt["exponent"] = "1.5";

    matched_from_distribution_density(fullRing, distribution_density_opt);

    f_vector_t v;
    util::read_vector_from_file(v, params + "dt.txt");

    ASSERT_EQ(v.size(), beam->dt.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = beam->dt[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of beam->dt failed on i " << i << endl;
    }

    util::read_vector_from_file(v, params + "dE.txt");

    ASSERT_EQ(v.size(), beam->dE.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = beam->dE[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of beam->dE failed on i " << i << endl;
    }

    delete long_tracker;
    delete fullRing;
}


TEST_F(testDistributions, matched_from_distribution_density2)
{
    auto RfP = Context::RfP;
    auto beam = Context::Beam;
    auto epsilon = 1e-8;
    auto params = string(TEST_FILES "/Distributions/distribution_density2/");

    auto long_tracker = new RingAndRfSection(RfP);
    vector<RingAndRfSection *> trackerList{long_tracker};
    auto fullRing = new FullRingAndRf(trackerList);

    map<string, string> distribution_density_opt;
    distribution_density_opt["type"] = "parabolic_line";
    distribution_density_opt["bunch_length"] = "200e-9";
    distribution_density_opt["density_variable"] = "density_from_J";
    distribution_density_opt["exponent"] = "2.0";

    matched_from_distribution_density(fullRing, distribution_density_opt);

    f_vector_t v;
    util::read_vector_from_file(v, params + "dt.txt");

    ASSERT_EQ(v.size(), beam->dt.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = beam->dt[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of beam->dt failed on i " << i << endl;
    }

    util::read_vector_from_file(v, params + "dE.txt");

    ASSERT_EQ(v.size(), beam->dE.size());
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = beam->dE[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of beam->dE failed on i " << i << endl;
    }

    delete long_tracker;
    delete fullRing;
}




class testBigaussian : public ::testing::Test {

protected:
    const long long N_b = 1e9;  // Intensity
    const double tau_0 = 0.4e-9; // Initial bunch length, 4 sigma [s]
    // Machine and RF parameters
    const double C = 26658.883;       // Machine circumference [m]
    const double p_i = 450e9;     // Synchronous momentum [eV/c]
    const double p_f = 460.005e9;     // Synchronous momentum, final
    const long long h = 35640;       // Harmonic number
    const double V = 6e6;             // RF voltage [V]
    const double dphi = 0;            // Phase modulation/offset
    const double gamma_t = 55.759505; // Transition gamma
    const double alpha =
        1.0 / gamma_t / gamma_t; // First order mom. comp. factor
    const int alpha_order = 1;
    const int n_sections = 1;
    // Tracking details

    const int N_t = 2000; // Number of turns to track
    const int N_p = 1000;  // Macro-particles
    const int N_slices = 10;

    virtual void SetUp()
    {
        omp_set_num_threads(1);

        f_vector_2d_t momentumVec(n_sections, f_vector_t(N_t + 1));
        for (auto &v : momentumVec)
            mymath::linspace(v.data(), p_i, p_f, N_t + 1);

        f_vector_2d_t alphaVec(n_sections, f_vector_t(alpha_order + 1, alpha));

        f_vector_t CVec(n_sections, C);

        f_vector_2d_t hVec(n_sections, f_vector_t(N_t + 1, h));

        f_vector_2d_t voltageVec(n_sections, f_vector_t(N_t + 1, V));

        f_vector_2d_t dphiVec(n_sections, f_vector_t(N_t + 1, dphi));

        Context::GP = new GeneralParameters(N_t, CVec, alphaVec,
                                            alpha_order, momentumVec,
                                            GeneralParameters::particle_t::proton);

        // Context::Beam = new Beams(N_p, N_b);

        auto GP = Context::GP;
        auto Beam = Context::Beam = new Beams(GP, N_p, N_b);

        auto RfP = Context::RfP = new RfParameters(GP, n_sections, hVec,
                voltageVec, dphiVec);

    }

    virtual void TearDown()
    {
        // Code here will be called immediately after each test
        // (right before the destructor).
        delete Context::GP;
        delete Context::Beam;
        delete Context::RfP;
    }

};

class testBigaussian2 : public ::testing::Test {

protected:
    // Bunch parameters
    const uint N_b = 0; // Intensity

    // Machine and RF parameters
    const double radius = 25;
    const double C = 2 * constant::pi * radius; // Machine circumference [m]
    const double p_i = 310891054.809;           // Synchronous momentum [eV/c]
    const uint h = 1;                          // Harmonic number
    const double V = 8000;                      // RF voltage [V]
    const double dphi = -constant::pi;          // Phase modulation/offset
    const double gamma_t = 4.076750841;         // Transition gamma
    const double alpha =
        1.0 / gamma_t / gamma_t; // First order mom. comp. factor
    const uint alpha_order = 1;
    const uint n_sections = 1;

    // Tracking details

    const int N_t = 500;    // Number of turns to track
    const int N_p = 500000; // Macro-particles
    const int N_slices = 10;

    virtual void SetUp()
    {
        omp_set_num_threads(1);

        f_vector_2d_t momentumVec(n_sections, f_vector_t(N_t + 1, p_i));

        f_vector_2d_t alphaVec(n_sections, f_vector_t(alpha_order + 1, alpha));

        f_vector_t CVec(n_sections, C);

        f_vector_2d_t hVec(n_sections, f_vector_t(N_t + 1, h));

        f_vector_2d_t voltageVec(n_sections, f_vector_t(N_t + 1, V));

        f_vector_2d_t dphiVec(n_sections, f_vector_t(N_t + 1, dphi));

        Context::GP = new GeneralParameters(N_t, CVec, alphaVec,
                                            alpha_order, momentumVec,
                                            GeneralParameters::particle_t::proton);

        // Context::Beam = new Beams(N_p, N_b);

        auto GP = Context::GP;
        auto Beam = Context::Beam = new Beams(GP, N_p, N_b);

        auto RfP = Context::RfP = new RfParameters(GP, n_sections, hVec,
                voltageVec, dphiVec);

    }

    virtual void TearDown()
    {
        // Code here will be called immediately after each test
        // (right before the destructor).
        delete Context::GP;
        delete Context::Beam;
        delete Context::RfP;
    }

};

TEST_F(testBigaussian, sigmas1)
{
    string params = TEST_FILES "/Distributions/Bigaussian/sigmas1/";
    auto epsilon = 1e-8;

    longitudinal_bigaussian(tau_0 / 4 , 0, -1, false);

    f_vector_t v;

    util::read_vector_from_file(v, params + "sigma_dE.txt");
    auto ref = v[0];
    auto real = Context::Beam->sigma_dE;
    ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)));

    util::read_vector_from_file(v, params + "sigma_dt.txt");
    ref = v[0];
    real = Context::Beam->sigma_dt;
    ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)));
}

TEST_F(testBigaussian, dE1)
{
    auto epsilon = 1e-8;

    string params = TEST_FILES "/Distributions/Bigaussian/dE1/";


    longitudinal_bigaussian(0.1 * tau_0, 0, -1, false);

    f_vector_t v;
    util::read_vector_from_file(v, params + "dE.txt");
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = Context::Beam->dE[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of dE failed on i " << i << '\n';

    }
}

TEST_F(testBigaussian, dE2)
{
    auto epsilon = 1e-8;

    string params = TEST_FILES "/Distributions/Bigaussian/dE2/";


    longitudinal_bigaussian(tau_0, 1e3, -1, false);

    f_vector_t v;
    util::read_vector_from_file(v, params + "dE.txt");
    for (uint i = 0; i < v.size(); ++i) {
        auto ref = v[i];
        auto real = Context::Beam->dE[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of dE failed on i " << i << '\n';

    }
}

TEST_F(testBigaussian, dt1)
{
    auto epsilon = 1e-8;

    auto Beam = Context::Beam;

    string params = TEST_FILES "/Distributions/Bigaussian/dt1/";

    longitudinal_bigaussian(tau_0, 1e3, -1, false);

    f_vector_t v;
    util::read_vector_from_file(v, params + "dt.txt");
    for (uint i = 0; i < v.size(); ++i) {
        double ref = v[i];
        double real = Beam->dt[i];
        ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
                << "Testing of dt failed on i " << i << '\n';

    }
}

TEST_F(testBigaussian2, dE3)
{
    auto epsilon = 1e-5;
    auto Beam = Context::Beam;

    string params = TEST_FILES "/Distributions/Bigaussian/dE3/";

    longitudinal_bigaussian(1e-9, 5e6, -1, false);

    f_vector_t v;

    util::read_vector_from_file(v, params + "dE_mean.txt");
    auto ref = v[0];
    auto real = mymath::mean(Beam->dE.data(), Beam->dE.size());
    ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
            << "Testing of Beam->dE_mean failed\n";

    util::read_vector_from_file(v, params + "dE_std.txt");
    ref = v[0];
    real = mymath::standard_deviation(Beam->dE.data(), Beam->dE.size());
    ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
            << "Testing of Beam->dE_std failed\n";
}

TEST_F(testBigaussian2, dt2)
{
    auto epsilon = 1e-5;

    auto Beam = Context::Beam;

    string params = TEST_FILES "/Distributions/Bigaussian/dt2/";

    longitudinal_bigaussian(1e-7, 5e4, -1, false);

    f_vector_t v;

    util::read_vector_from_file(v, params + "dt_mean.txt");
    auto ref = v[0];
    auto real = mymath::mean(Beam->dt.data(), Beam->dt.size());
    ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
            << "Testing of Beam->dt_mean failed\n";

    util::read_vector_from_file(v, params + "dt_std.txt");
    ref = v[0];
    real = mymath::standard_deviation(Beam->dt.data(), Beam->dt.size());
    ASSERT_NEAR(ref, real, epsilon * max(abs(ref), abs(real)))
            << "Testing of Beam->dt_std failed\n";
}



int main(int ac, char *av[])
{
    python::initialize();
    ::testing::InitGoogleTest(&ac, av);
    auto ret = RUN_ALL_TESTS();
    python::finalize();
    return ret;
}
