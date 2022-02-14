// Copyright © 2021 Jonas Muehlmann
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <string>

#include "frontend/semantic_analysis/semantic_error.hpp"
#include "nlohmann/json.hpp"
#include "source_location.hpp"

using json = nlohmann::ordered_json;

struct semantic_error_stringifier_visitor
{
    std::string operator()(const call_to_variable& error)
    {
        json violating_call;

        to_json(violating_call, error.violating_call);

        return "Call to variable at " + violating_call.dump(4);
    }

    std::string operator()(const assignment_to_callable& error)
    {
        json violating_assignment;

        to_json(violating_assignment, error.violating_assignment);

        return "Assignment to callable at " + violating_assignment.dump(4);
    }

    std::string operator()(const evaluatio_of_procedure& error)
    {
        json violating_evaluation;
        to_json(violating_evaluation, error.violating_evaluation);
        return "Evaluation of procedure call at " + violating_evaluation.dump(4);
    }

    std::string operator()(const redifintion_of_symbol& error)
    {
        json violating_definition;
        json definition;

        to_json(violating_definition, error.violating_definition);
        to_json(definition, error.previous_definition);

        return "Redifinition of symbol at " + violating_definition.dump(4)
               + " with previous definition at " + definition.dump(4);
    }

    std::string operator()([[maybe_unused]] const missing_main_function& error)
    {
        return "Missing main function";
    }

    std::string operator()(const use_before_definition& error)
    {
        json violating_use;
        json definition;

        to_json(violating_use, error.violating_use);
        to_json(definition, error.definition);

        return "Use of symbol " + violating_use.dump(4) + " with definition at "
               + definition.dump(4);
    }
};
