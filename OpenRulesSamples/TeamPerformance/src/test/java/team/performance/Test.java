package team.performance;

import com.openrules.core.DecisionModel;
import com.openrules.core.Goal;

public class Test {
    
    public static void main(String[] args) {
           
        DecisionModel model = new DecisionModelTeamPerformance();
        Goal goal = model.createGoal();
        goal.put("Report", "On");
        Team[] teams = teamsArray.get();
        for (Team team : teams) {
            System.out.println("\n=== Test " + team.getTeamName() + " ===");
            long startTime = System.currentTimeMillis();
            goal.use("Team", team);
            team.setTeamScore(0);
            goal.execute();
            System.out.println("TEAM SCORE=" + team.getTeamScore());
            long endTime = System.currentTimeMillis();
            System.out.println("Elapsed time: " + (endTime - startTime +1) + " ms");
        }        
    }

}
