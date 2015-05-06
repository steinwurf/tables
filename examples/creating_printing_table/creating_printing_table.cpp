// Copyright (c) 2015 Steinwurf ApS
// All Rights Reserved
//
// Signed-off-to-Steinwurf-by: Juan Cabrera <jcabre14@student.aau.dk>
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.


#include <fstream>
#include <cstdint>

#include <tables/table.hpp>
#include <tables/csv_format.hpp>
#include <tables/json_format.hpp>
#include <tables/python_format.hpp>

/// @example creating_printing_table.cpp
///
/// Simple example showing how to create a table
/// and print it in any desired format.

int main()
{

    // In this example we will generate 100 random numbers. We then
    // will store those numbers in a table with two columns:
    // The random number and the iteration in wich it was generated.

    // We set the names for the ouput files.
    std::string out_csv = "out.csv";
    std::string out_json = "out.json";
    std::string out_python = "out_python.json";

    // We will make the table object where the
    // results are going to be stored.
    tables::table results;

    // We first create the two columns of the table.
    if (!results.has_column("Iteration"))
        results.add_column("Iteration");

    if (!results.has_column("Number"))
        results.add_column("Number");

    for (uint8_t i = 0; i < 100; ++i)
    {
        // Generate the random number.
        uint32_t random_number = rand();

        // Add a row to the table.
        results.add_row();

        // Store the generated number and the iteration
        // in the table.
        results.set_value("Iteration", i);
        results.set_value("Number", random_number);
    }


    // We create the ofstream objects
    std::ofstream ofc(out_csv);
    std::ofstream ofj(out_json);
    std::ofstream ofp(out_python);

    // We create the printers objects, each one will print
    // in an output file the table in a different format.
    tables::csv_format printer_csv;
    tables::json_format printer_json;
    tables::python_format printer_python;

    // We call the print methods
    printer_csv.print(ofc, results);
    printer_json.print(ofj, results);
    printer_python.print(ofp, results);

    return 0;

}