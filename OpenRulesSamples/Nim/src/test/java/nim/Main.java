package nim;

import java.util.Arrays;

import com.openrules.core.DecisionModel;
import com.openrules.core.Goal;

public class Main {

    public static void main(String[] args) {
        DecisionModel model = new DecisionModelNim();
        Goal goal = model.createGoal();
        goal.put("trace", "Off");
        Nim[] games = gameArray.get();
        String[] output = new String[games.length];
        
        for (int i = 0; i < games.length-1; i++) {
            Nim game = games[i];
            goal.use("Nim", game);
            goal.execute();
            output[i] = "Remains:"+game.getNumberOfRemainingObjects()
                    +" Take: " + Arrays.toString(game.getTakeObjectsWithNumbers()) 
                    +" " + game.getResult();
        }
        for (int i = 0; i < games.length-1; i++) {
            System.out.println(output[i]);
        }
    }
}

