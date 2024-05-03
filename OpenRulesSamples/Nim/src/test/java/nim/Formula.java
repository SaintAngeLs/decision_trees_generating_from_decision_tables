package nim;

import java.util.Arrays;

import com.openrules.core.DecisionModel;
import com.openrules.core.Goal;

public class Formula {

    public static void main(String[] args) {
        DecisionModel model = new DecisionModelNim();
        Goal goal = model.createGoal();
        Nim[] games = gameArray.get();
        String[] output = new String[games.length];
        for (int i = 0; i < games.length-1; i++) {
            Nim game = games[i];
            goal.use("Nim", game);
            goal.execute();
            output[i] = "Remains:"+game.getNumberOfRemainingObjects()
                    +" Take: " + Arrays.toString(game.getTakeObjectsWithNumbers()) 
                    +" You are " + game.getResult();
        }
        for (int i = 0; i < games.length-1; i++) {
            System.out.println(output[i]);
        }
    }
}

// We could have been clever and proved that if n=4*k+1 with k integer then the 
