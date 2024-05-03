package com.userv;

import com.openrules.core.Goal;

public class Main {

	public static void main(String[] args) {
		
        DecisionModelInsurancePremium model = new DecisionModelInsurancePremium();
        Goal goal = model.createGoal("CalculateClientPremium");
//        goal.put("Report", "On");
//        goal.put("trace", "On");
		
        Driver[] drivers = driversArray.get();
		Car[] cars = carsArray.get();
		Client[] clients = clientsArray.get();
		Client client = clients[0];
		client.setDrivers(drivers);
		client.setCars(cars);
		goal.use("Client", client);
		goal.execute();
	}
}
