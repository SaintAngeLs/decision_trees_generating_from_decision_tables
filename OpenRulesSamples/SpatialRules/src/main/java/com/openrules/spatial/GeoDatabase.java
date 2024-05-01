package com.openrules.spatial;

/**
 * This is a singleton that keeps all geometries
 */

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.openrules.core.Decision;
import com.vividsolutions.jts.geom.Geometry;

public class GeoDatabase {
    
    private static Logger Log = LoggerFactory.getLogger(GeoDatabase.class);
	
	static List<Geometry> countyGeometries = null;
	static List<Geometry> hrrGeometries = null;
	static List<Geometry> hsaGeometries = null;
	static List<Geometry> hospitalGeometries = null;
	
	public static List<Geometry> getCountyGeometries() {
		return countyGeometries;
	}
	
	public static List<Geometry> getHrrGeometries() {
		return hrrGeometries;
	}

	public static List<Geometry> getHsaGeometries() {
		return hsaGeometries;
	}

	public static List<Geometry> getHospitalGeometries() {
		return hospitalGeometries;
	}

	public static List<Geometry> getCountyGeometries(String filename) {
		if (countyGeometries == null)
			readCountyGeometries(filename);
		return countyGeometries;
	}
	
	static void readCountyGeometries(String filename) {
		Log.info("Reading " + filename);
		countyGeometries = ReadGML.read(filename);
	}
	
	public static List<Geometry> getHRRGeometries(String filename) {
		if (hrrGeometries == null)
			readHRRGeometries(filename);
		return hrrGeometries;
	}
	
	static void readHRRGeometries(String filename) {
		Log.info("Reading " + filename);
		hrrGeometries = ReadGML.read(filename);
	}
	
	public static List<Geometry> getHSAGeometries(String filename) {
		if (hsaGeometries == null)
			readHSAGeometries(filename);
		return hsaGeometries;
	}
	
	static void readHSAGeometries(String filename) {
		//String filename = ".\\data\\hsa_gml.gml";
		Log.info("Reading " + filename);
		hsaGeometries = ReadGML.read(filename);
	}
	
	public static List<Geometry> getHospitalGeometries(String filename) {
		if (hospitalGeometries == null)
			readHospitalGeometries(filename);
		return hospitalGeometries;
	}
	
	static void readHospitalGeometries(String filename) {
		//String filename = ".\\data\\hospital_gml.gml";
		Log.info("Reading " + filename);
		hospitalGeometries = ReadGML.read(filename);
	}
	
	static public boolean conditionEntityToEntity( Decision decision,
			String mainEntityType, String relationship, String relatedEntityType,
			String oper, String value) {
		Log.info("Evaluate: " + mainEntityType + " " + relationship + " "
				+ relatedEntityType + " " + oper + " " + value);
		if (relationship ==null) {
			Log.error("ERROR in conditionEntityToEntity: Relationship not defined");
			return false;
		}
		GeoEntity mainEntity = (GeoEntity)decision.getBusinessObject("MainEntity");
		if (mainEntity == null) {
			Log.error("ERROR in conditionEntityToEntity: Main Entity not defined");
			return false;
		}
		if (!mainEntity.getType().equalsIgnoreCase(mainEntityType))
			return false;
		
		GeoEntity relatedEntity = (GeoEntity)decision.getBusinessObject("RelatedEntity");
		if (relatedEntity == null) {
			Log.info("WARNING from conditionEntityToEntity: Related Entity not defined");
			if (!relationship.equalsIgnoreCase("Area"))
				return false;
		}
		if (!relatedEntity.getType().equalsIgnoreCase(relatedEntityType)) {
			if (!relationship.equalsIgnoreCase("Area"))
				return false;
		}
		
		return mainEntity.validateRelationshipToEntity(relationship, relatedEntity, oper, value);
	}
	
	static public boolean conditionEntityToCountOfRelatedEntities( Decision decision,
			String mainEntityType, String relationship, String relatedEntityType,
			String oper, String value) {
		Log.info("Evaluate: " + mainEntityType + " " + relationship + " " 
				+ relatedEntityType + " " + oper + " " + value);
		GeoEntity mainEntity = (GeoEntity)decision.getBusinessObject("MainEntity");
		if (mainEntity == null) {
			Log.error("ERROR in conditionEntityToEntity: Main Entity not defined");
			return false;
		}
		if (!mainEntity.getType().equalsIgnoreCase(mainEntityType))
			return false;
		
		return mainEntity.validateRelationshipToCountOfRelatedEntities(relationship, relatedEntityType, oper, value);
	}
	
	static public List<Geometry> getGeometryList(String entityType) {
		List<Geometry> list = null;
		if (entityType.equalsIgnoreCase("County"))
			list = GeoDatabase.getCountyGeometries();
		else if (entityType.equalsIgnoreCase("HRR"))
			list = GeoDatabase.getHrrGeometries();
		else if (entityType.equalsIgnoreCase("HSA"))
			list = GeoDatabase.getHsaGeometries();
		else if (entityType.equalsIgnoreCase("Hospital"))
			list = GeoDatabase.getHrrGeometries();
		else {
			Log.info("Entity type " + entityType + " is not supported yet");
			return null;
		}
		return list; 
	}
					
	

}
