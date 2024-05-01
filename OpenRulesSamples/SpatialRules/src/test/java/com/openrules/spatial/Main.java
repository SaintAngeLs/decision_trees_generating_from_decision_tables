package com.openrules.spatial;

/**
 * This is a singleton that keeps all entities
 */

import com.openrules.core.*;

import static java.lang.System.out;

import java.util.Arrays;

public class Main {

	public static void main(String[] args) {
		System.out.println(Arrays.asList(args));
		// Build EntityRepository from GML files
		EntityRepository.build();

		// Create DecisionModel
		DecisionModelSpatial model = new DecisionModelSpatial();
	    Goal goal = model.createGoal();
	    goal.put("report", "On");
	    
		GeoEntity mainEntity = EntityRepository.hrrs[0];
		GeoEntity relatedEntity = EntityRepository.hsas[0];
		run(goal,"FIRST RUN", mainEntity, relatedEntity);
		
		mainEntity = EntityRepository.hospitals[0];
		relatedEntity = EntityRepository.hsas[0];
		run(goal,"SECOND RUN", mainEntity, relatedEntity);
		
		mainEntity = EntityRepository.hrrs[1];
		relatedEntity = EntityRepository.hospitals[10];
		run(goal,"THIRD RUN", mainEntity, relatedEntity);
		
		Facility f1 = new Facility();
		f1.setId("Facility 1");
		Facility f2 = new Facility();
		f2.setId("Facility 2");
		Facility f3 = new Facility();
		f3.setId("Facility 3");
		University u = new University();
		u.setId("University 1");
		Facility[] facilities = { f1, f2, f3 };
		u.setFacilities(facilities);
		mainEntity = f2;
		relatedEntity = u;
		run(goal,"FOURTH RUN", mainEntity, relatedEntity);
	}
	
	static void run(Goal goal, String name, GeoEntity mainEntity, GeoEntity relatedEntity) {
		long executionStart = System.currentTimeMillis();
		out.println("\n" + name);
		out.println("Main Entity: " + mainEntity); 
		out.println("Related Entity: " + relatedEntity); 
		goal.use("MainEntity", mainEntity);
		goal.use("RelatedEntity", relatedEntity);
		Response response = new Response();
		goal.use("Response", response);
		goal.execute();
		out.println("=== After decision's run: SpatialSignificanceScore = " + response.getSpatialSignificanceScore());
		long executionTime = System.currentTimeMillis() - executionStart;
		out.println("Decision run execution time: " + executionTime + " msec");
	}
}

