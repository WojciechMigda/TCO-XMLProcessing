#include "model_params.hpp"
#include "infer.hpp"
#include "train.hpp"

#include "spdlog/spdlog.h"
#include "clipp.hpp"

#include <iostream>
#include <fstream>


struct is_natural
{
    bool operator()(std::string const & arg) const
    {
        unsigned int val = 0;

        try
        {
            val = std::stoi(arg);
        }
        catch (std::invalid_argument const & e)
        {
            return false;
        }

        return val > 0;
    }
};


struct is_nonnegative
{
    bool operator()(std::string const & arg) const
    {
        unsigned int val = 0;

        try
        {
            val = std::stoi(arg);
        }
        catch (std::invalid_argument const & e)
        {
            return false;
        }

        return val >= 0;
    }
};


bool bad_filename_r(std::string const & fname)
{
    std::ifstream f(fname);

    bool ok = f.is_open();

    if (not ok)
    {
        spdlog::error("Cannot open file {}", fname);
    }

    return not ok;
}


bool bad_filename_w(std::string const & fname)
{
    std::ofstream f(fname, std::ios::app);

    bool ok = f.is_open();

    if (not ok)
    {
        spdlog::error("Cannot open file for writing {}", fname);
    }

    return not ok;
}


int main(int argc, char **argv)
{
    unsigned int ts_threshold = 8000;
    float ts_s = 4.0;
    unsigned int ts_clauses = 3200;
    int ts_njobs = 2;
    int ts_max_weight = std::numeric_limits<int>::max();
    bool boost_tpf = true;
    unsigned int nepochs = 20u;
    std::string model_ifname;
    std::string model_ofname;
    std::string csv_ifname;
    std::string encoder_ifname;
    std::string infer_ofname;
    bool do_infer = false;
    bool do_regression = false;
    std::string ts_loss_fn = "MSE";
    float ts_loss_fn_C1 = 0.f;
    bool f201906 = false;
    bool fseasonal = true;


    auto inference = (
        clipp::command("infer").set(do_infer, true).doc("Request inference using existing model"),
        clipp::required("--datafile", "-d").doc("Input CSV file for training or inference") & clipp::value("CSV data file to read from", csv_ifname),
        clipp::required("--encoding", "-e").doc("Input JSON file with input encoding parameters") & clipp::value("JSON encoding file to read from", encoder_ifname),
        clipp::required("--model", "-m").doc("Pre-trained input model to load") & clipp::value("JSON-ized model file to read from", model_ifname),
        clipp::required("--output", "-o").doc("Output CSV file where raw inference results will be written") & clipp::value("Output CSV file", infer_ofname),
        clipp::option("--f201906").set(f201906, true).doc("Enable encoding YYYYMM >= 201906, default=" + std::to_string(f201906)),
        (
            clipp::option("--fseasonal").set(fseasonal, true) |
            clipp::option("--fno-seasonal").set(fseasonal, false)
        ).doc("Enable/disable encoding monthly seasonal features, default=" + std::to_string(fseasonal))
    );

    auto training = (
        clipp::command("train").set(do_infer, false).doc("Request model training / augmenting"),
        clipp::required("--datafile", "-d").doc("Input CSV file for training or inference") & clipp::value("CSV data file to read from", csv_ifname),
        clipp::required("--encoding", "-e").doc("Input JSON file with input encoding parameters") & clipp::value("JSON encoding file to read from", encoder_ifname),
        clipp::option("--model", "-m").doc("Pre-trained input model to load") & clipp::value("JSON-ized model file to read from", model_ifname),
        clipp::required("--output-model", "-o").doc("Output file to save the trained model") & clipp::value("JSON-ized model file to save to", model_ofname),

        clipp::option("--tsetlini-threshold", "-T").doc("Threshold parameter for the Tsetlini model") & clipp::value(is_natural(), "threshold=" + std::to_string(ts_threshold), ts_threshold),
        clipp::option("--tsetlini-s", "-s").doc("Specificity parameter for the Tsetlini model") & clipp::value("s=" + std::to_string(ts_s), ts_s),
        clipp::option("--tsetlini-clauses", "-C").doc("Number of clauses parameter for the Tsetlini model") & clipp::value(is_natural(), "clauses=" + std::to_string(ts_clauses), ts_clauses),
        clipp::option("--tsetlini-loss-fn", "-L").doc("Loss function selection parameter for the Tsetlini model") & clipp::value("loss_fn=" + ts_loss_fn, ts_loss_fn),
        clipp::option("--tsetlini-loss-fn-C1", "-C1").doc("Loss function C1 parameter for the Tsetlini model") & clipp::value("loss_fn_C1=" + std::to_string(ts_loss_fn_C1), ts_loss_fn_C1),
        (
            clipp::option("--tsetlini-boost-tpf", "-B").set(boost_tpf, true) |
            clipp::option("--tsetlini-no-boost-tpf", "-b").set(boost_tpf, false)
        ).doc("Boost (T)true (P)ositive (F)eedback parameter for the Tsetlini model, default=" + std::to_string(boost_tpf)),
        clipp::option("--tsetlini-max-weight", "-w").doc("Max weight parameter for the Tsetlini model") & clipp::value(is_natural(), "max_weight=" + std::to_string(ts_max_weight), ts_max_weight),
        clipp::option("--tsetlini-jobs", "-j").doc("Number of jobs parameter for the Tsetlini model") & clipp::value("jobs=" + std::to_string(ts_njobs), ts_njobs),
        clipp::option("--nepochs").doc("Number of epochs for each call to fit()") & clipp::value(is_natural(), "nepochs=" + std::to_string(nepochs), nepochs),
        clipp::option("--f201906").set(f201906, true).doc("Enable encoding YYYYMM >= 201906, default=" + std::to_string(f201906)),
        (
            clipp::option("--fseasonal").set(fseasonal, true) |
            clipp::option("--fno-seasonal").set(fseasonal, false)
        ).doc("Enable/disable encoding monthly seasonal features, default=" + std::to_string(fseasonal)),
        (
            clipp::option("--tsetlini-regressor", "-r").set(do_regression, true) |
            clipp::option("--tsetlini-classifier", "-c").set(do_regression, false)
        ).doc("Select classification vs. regression, default=" + std::to_string(do_regression))
    );

    auto cli = (inference | training);

    if (not clipp::parse(argc, argv, cli))
    {
        std::cout << clipp::make_man_page(cli, argv[0]);
    }
    else
    {
        spdlog::info("Start");

        if (do_infer)
        {
            spdlog::info("Inference");

            if (bad_filename_r(csv_ifname)
                or bad_filename_r(encoder_ifname)
                or bad_filename_r(model_ifname)
                or bad_filename_w(infer_ofname))
            {
                std::exit(1);
            }

//            infer(csv_ifname, encoder_ifname, model_ifname, infer_ofname,
//                f201906, fseasonal);
        }
        else
        {
            spdlog::info("Training");

            if (bad_filename_r(csv_ifname)
                or bad_filename_r(encoder_ifname)
                or bad_filename_w(model_ofname))
            {
                std::exit(1);
            }

            model_params_t const model_params{
                {"regression", do_regression},
                {"threshold", ts_threshold},
                {"s", ts_s},
                {"clauses", ts_clauses},
                {"boost_tpf", boost_tpf},
                {"loss_fn", ts_loss_fn},
                {"loss_fn_C1", ts_loss_fn_C1},
                {"max_weight", ts_max_weight},
                {"nepochs", nepochs},
                {"n_jobs", ts_njobs}
            };

//            train(csv_ifname, encoder_ifname, model_ifname, model_ofname, model_params,
//                f201906, fseasonal);
        }
    }

    return 0;
}
