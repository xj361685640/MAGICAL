/**
 * @file top_floorplan.h
 * @brief The floorplan for top-level system integration
 * @author Keren Zhu
 * @date 10/05/2020
 */

#pragma once

#include "db/DesignDB.h"
#include "util/linear_programming.h"
#include "util/Box.h"
#include "constraint_graph.hpp"

PROJECT_NAMESPACE_BEGIN

/// @brief the top-level problem
class TopFloorplanProblem
{
    friend class IlpTopFloorplanProblem;
    public:
        explicit TopFloorplanProblem() = default;
        /// @brief intialize a problem from CktGraph
        void initProblem(const DesignDB& dDb, const CktGraph &ckt, const std::string& symnetFile);
    private:
        enum class FpPinType
        {
            ASYM = 0,
            SYM_PRI = 1, // The primary one in the sym pair
            SYM_SCE = 2, // The secondary one in the sym pair
            OTHER // Don't need to care
        };
        struct PinIdx
        {
            FpPinType pinType; 
            IndexType idx;
            IndexType cellIdx;
        };
        struct FpNet
        {
            std::vector<IndexType> pins;
        };
        /* Problem   */
        std::vector<PinIdx> _pinIdx; ///< To relate the pin in CktGraph to the variables
        std::vector<Box<LocType>> _cellBBox; ///< The original cell layout bounding boxes
        IntType resourcePerLen = 1000; ///< To quanitfy the pin resource per length
        std::vector<FpNet> _nets; ///< The nets
        IndexType _numAsymPins = 0; ///< The number of asym pins need to assign
        IndexType _numSymPriPins = 0; ///< The number of primary sym pins need to assign
        IndexType _numSymSecPins = 0; ///< The number of primary sym pins need to assign

};

/// @brief the ilp for solving the TopFloorplanProblem
class IlpTopFloorplanProblem
{
    public:
        explicit IlpTopFloorplanProblem(const TopFloorplanProblem &problem)
            : _problem(problem) {}
        /// @brief solve the problem
        /// @return true: successful.
        bool solve();
    private:
        /// @brief sweep line to generate the vertical constraint graph
        void verticalSweepLine();
    private:
        std::vector<constraint_graph::ConstraintEdge> _verConstrGraph; ///< The vertical constraint graph
        const TopFloorplanProblem &_problem;

};

PROJECT_NAMESPACE_END
