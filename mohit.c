// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract TravelPlanning {
    struct Plan {
        address proposer;
        string destination;
        uint256 cost;
        uint256 votes;
        bool finalized;
    }

    address public admin;
    Plan[] public plans;
    mapping(address => bool) public voters;
    uint256 public votingThreshold;
    uint256 public proposalCount;
    
    constructor(uint256 _votingThreshold) {
        admin = msg.sender;
        votingThreshold = _votingThreshold;
    }

    modifier onlyAdmin() {
        require(msg.sender == admin, "Not authorized");
        _;
    }

    modifier hasNotVoted() {
        require(!voters[msg.sender], "Already voted");
        _;
    }

    function proposePlan(string memory _destination, uint256 _cost) public {
        plans.push(Plan({
            proposer: msg.sender,
            destination: _destination,
            cost: _cost,
            votes: 0,
            finalized: false
        }));
        proposalCount++;
    }

    function voteOnPlan(uint256 planId) public hasNotVoted {
        require(planId < plans.length, "Invalid plan ID");
        require(!plans[planId].finalized, "Plan already finalized");

        plans[planId].votes++;
        voters[msg.sender] = true;

        if (plans[planId].votes >= votingThreshold) {
            plans[planId].finalized = true;
        }
    }

    function getPlan(uint256 planId) public view returns (address, string memory, uint256, uint256, bool) {
        require(planId < plans.length, "Invalid plan ID");
        Plan memory plan = plans[planId];
        return (plan.proposer, plan.destination, plan.cost, plan.votes, plan.finalized);
    }

    function resetVotes() public onlyAdmin {
        for (uint256 i = 0; i < plans.length; i++) {
            plans[i].votes = 0;
            plans[i].finalized = false;
        }
        delete voters;
    }
}
