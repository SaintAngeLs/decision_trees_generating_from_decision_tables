package com.openrules.spatial;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.operation.distance.DistanceOp;

public abstract class GeoEntity {
    
    private static Logger Log = LoggerFactory.getLogger(GeoEntity.class);

	private String id;
	private String name;
	private Geometry geometry;

	public GeoEntity() {
		super();
		id = null;
		name = null;
		geometry = null;
	}

	public GeoEntity(int i, Geometry geometry) {
		super();
		setId("" + (i + 1));
		setGeometry(geometry);
	}

	// public GeoEntity(Geometry geometry) {
	// super();
	// id = null;
	// name = null;
	// geometry = null;
	// }

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public Geometry getGeometry() {
		return geometry;
	}

	public void setGeometry(Geometry geometry) {
		this.geometry = geometry;
	}

	public boolean isSpatial() {
		return geometry != null;
	}

	abstract public String getType();

	@Override
	public String toString() {
		return "GeoEntity [type=" + getType() + " id=" + id + 
				", coordinates=" + geometry.getCoordinates().length + ", points="
				+ geometry.getNumPoints() + "]";
	}

	/**
	 * 
	 * @param geoEntity
	 * @return the distance between the closest points of this entity and the
	 *         entity passes as a parameter
	 */
	public double smallestDistance(GeoEntity geoEntity) {
		if (this.isSpatial() && geoEntity.isSpatial()) {
			return DistanceOp.distance(this.getGeometry(),
					geoEntity.getGeometry());
		} else {
			return Double.MAX_VALUE;
		}
	}

	/**
	 * 
	 * @param thisType
	 * @param relationship
	 * @param relatedType
	 * @param oper
	 * @param value
	 * @return
	 */
	public boolean validateRelationshipToEntity(String relationship,
			GeoEntity entity, String oper, String value) {
		//Log.info("Validate relationship " + relationship);
		// Analyze "relationship"
		if (relationship == null) {
			Log.error("The relationship between GeoEntities " + this.getName()
					+ " and " + entity.getName()
					+ " is not defined: condition assumed FALSE");
			return false;
		}
		if (oper == null) {
			Log.error("The relationship between GeoEntities " + this.getName()
					+ " and " + entity.getName() + " assumes oper 'Is'");
			oper = "Is";
		}
		if (value == null) {
			Log.error("The relationship between GeoEntities " + this.getName()
					+ " and " + entity.getName() + " assumes value 'TRUE'");
			value = "TRUE";
		}
		relationship = relationship.trim();
		oper = oper.trim();

		// SPATIAL RELATIONSHIPS
		Geometry thisGeomentry = getGeometry();
		Geometry relatedGeometry = entity.getGeometry();

		boolean b = false;
		boolean defined = false;
		// Equals ("Hospital Service Area A equals Hospital Service Area B")
		if (relationship.equalsIgnoreCase("Equals")) {
			b = thisGeomentry.equals(relatedGeometry);
		}

		// Disjoint
		// ("Hospital Service Area A is disjoint from Hospital Service Area B")
		if (relationship.equalsIgnoreCase("Disjoint")
				|| relationship.equalsIgnoreCase("Is Disjoint From")) {
			b = thisGeomentry.disjoint(relatedGeometry);
			defined = true;
		} else
		// Touches ("Hospital Service Area A touches Hospital Service Area B")
		if (relationship.equalsIgnoreCase("Touches")) {
			b = thisGeomentry.touches(relatedGeometry);
			defined = true;
		} else
		// Contains
		// ("Hospital Referral Region A contains Hospital Service Area B")
		if (relationship.equalsIgnoreCase("Contains")) {
			b = thisGeomentry.contains(relatedGeometry);
			defined = true;
		} else
		// Covers ("Hospital Referral Region A covers Hospital Service Area B")
		if (relationship.equalsIgnoreCase("Covers")) {
			b = thisGeomentry.covers(relatedGeometry);
			defined = true;
		} else
		// Intersects
		// ("Hospital Referral Area A intersects Hospital Service Area B")
		if (relationship.equalsIgnoreCase("Intersects")) {
			b = thisGeomentry.intersects(relatedGeometry);
			defined = true;
		} else
		// Within ("Residence A is within Hospital Service Area B")
		if (relationship.equalsIgnoreCase("Within")) {
			b = thisGeomentry.within(relatedGeometry);
			defined = true;
		} else
		// CoveredBy ("Residence A is covered by Hospital Service Area B")
		if (relationship.equalsIgnoreCase("CoveredBy")) {
			b = thisGeomentry.coveredBy(relatedGeometry);
			defined = true;
		} else
		// Crosses ("Evacuation Route A crosses Evacuation Route B")
		if (relationship.equalsIgnoreCase("Crosses")) {
			b = thisGeomentry.crosses(relatedGeometry);
			defined = true;
		} else
		// Overlaps
		// ("Hospital Referral Region A overlaps Hospital Referral Region B")
		if (relationship.equalsIgnoreCase("Overlaps")) {
			b = thisGeomentry.overlaps(relatedGeometry);
			defined = true;
		}

		if (defined) {
			if ("Is".equalsIgnoreCase(oper) && "True".equalsIgnoreCase(value))
				return b == true;
			if ("Is".equalsIgnoreCase(oper) && "False".equalsIgnoreCase(value))
				return b == false;
			if ("Is Not".equalsIgnoreCase(oper)
					&& "True".equalsIgnoreCase(value))
				return b == false;
			if ("Is Not".equalsIgnoreCase(oper)
					&& "False".equalsIgnoreCase(value))
				return b == true;
			Log.error("The relationship between GeoEntities "
					+ this.getName()
					+ " and "
					+ entity.getName()
					+ " have invalid Oper and/or Value: condition assumed FALSE");
			Log.error("Expected | Oper=Is/Is Not | Value=TRUE/FALSE |");
			return false;
		}

		// Distance or Area
		if (relationship.equalsIgnoreCase("Distance")
				|| relationship.equalsIgnoreCase("Area")) {
			return validateRelationshipDistanceOrArea(relationship,entity,oper,value);
		}

		// HRR Is Among 5 Closest To Airport or Airport Is Among 5 Farthest To Hospital
		if (relationship.startsWith("Is Among")) {
			return validateRelationshipIsAmong(relationship,entity);		
		}

		// NON-SPATIAL RELATIONSHIPS

		// Is Part Of ("Facility Is Part Of University")
		if (relationship.equalsIgnoreCase("Is Part Of")) {
			if (this.getType().equals("Facility") && entity.getType().equals("University")) {
				University u = (University)entity;
				Facility f = (Facility)this;
				return u.includes(f);
			}
			Log.info("The relationship 'Is Part Of' is implemented only for Universities and Facilities");
			return false;
		}

		// Comprises ("University Comprises Facility")
		if (relationship.equalsIgnoreCase("Comprises")) {
			if (this.getType().equals("University") && entity.getType().equals("Facility")) {
				University u = (University)this;
				Facility f = (Facility)entity;
				return u.includes(f);
			}
			Log.info("The relationship 'Comprises' is implemented only for Universities and Facilities");
			return false;
		}

		Log.info("The unknown geo-spatial relationship: " + relationship);
		return false;
	}
	
	
	/*
	 * Distance or Area
	 */
	public boolean validateRelationshipDistanceOrArea(String relationship, GeoEntity entity, 
			String oper, String value) {
		Geometry thisGeomentry = getGeometry();
		Geometry relatedGeometry = entity.getGeometry();
		double calculatedValue;
		if (relationship.equalsIgnoreCase("Distance")) {
			calculatedValue = thisGeomentry.distance(relatedGeometry);
			Log.info("Distance = " + calculatedValue);
		} else {
			calculatedValue = thisGeomentry.getArea();
			Log.info("Area = " + calculatedValue);
		}
		try {
			double dd = Double.parseDouble(value);
			if ("<".equals(oper))
				return calculatedValue < dd;
			else if ("<=".equals(oper))
				return calculatedValue <= dd;
			else if ("==".equals(oper) || "=".equals(oper)
					|| "Is".equalsIgnoreCase(oper))
				return Math.abs(calculatedValue - dd) < 0.001;
			else if (">=".equals(oper))
				return calculatedValue >= dd;
			else if ("<".equals(oper))
				return calculatedValue < dd;
			else {
				Log.error("The relationship " + relationship
						+ " expects a comparison operator and not " + oper
						+ ". Condition assumed FALSE");
				return false;
			}

		} catch (Exception e) {
			Log.error("The relationship " + relationship
					+ " expects a numeric Value: not " + value
					+ ". Condition assumed FALSE");
			return false;
		}
	}

	/*
	 * HRR Is Among 5 Closest To Airport or Airport Is Among 5 Farthest To Hospital
	 */
	public boolean validateRelationshipIsAmong(String relationship, GeoEntity entity) {
		Geometry thisGeomentry = getGeometry();
		Geometry relatedGeometry = entity.getGeometry();
		// Log.info("The relationship 'Is Among <n> Closest To' not implemented yet");
		String limitString = "";
		for (int i = 9; i < 12; i++) {
			Character c = relationship.charAt(i);
			if (Character.isDigit(c))
				limitString += c;
			else
				break;
		}
		if (limitString == "") {
			Log.error("The relationship " + relationship
					+ " expects a numeric limit after 'Is Among'"
					+ ". Condition assumed FALSE");
			return false;
		}
		int limit = Integer.parseInt(limitString);
		boolean closest = false; // Farthest
		if (relationship.indexOf("Closest") > 0
				|| relationship.indexOf("closest") > 0)
			closest = true;
		double thisDistance = thisGeomentry.distance(relatedGeometry);
		int n = 0;
		List<Geometry> list = GeoDatabase.getGeometryList(this.getType());
		if (list == null) {
			return false;
		}
		for (int i = 0; i < list.size(); i++) {
			// Log.info("i=" + i + " n=" + n);
			Geometry iGeometry = list.get(i);
			if (iGeometry == thisGeomentry)
				continue;
			double iDistance = thisGeomentry.distance(relatedGeometry);
			if (closest) {
				if (iDistance < thisDistance) {
					n++;
					if (n > limit)
						return false;
				}
			} else {
				if (iDistance > thisDistance) {
					n++;
					if (n > limit)
						return false;
				}
			}
		}
		return true;
	}

	public boolean validateRelationshipToCountOfRelatedEntities(
			String relationship, String entityType, String oper, String value) {
		// Analyze "relationship"
		if (relationship == null) {
			Log.error("The relationship between GeoEntity " + this.getName()
					+ " and a count of related entities of the type "
					+ entityType + " is not defined: condition assumed FALSE");
			return false;
		}

		if (oper == null) {
			Log.error("The relationship between GeoEntity " + this.getName()
					+ " and a count of related entities of the type "
					+ entityType + " has not defined Oper: 'Is' is assumed");
			oper = "Is";
		}
		if (value == null) {
			Log.error("The relationship between GeoEntity " + this.getName()
					+ " and a count of related entities of the type "
					+ entityType
					+ " has undefined Value: condition assumed FALSE");
			return false;
		}
		relationship = relationship.trim();
		oper = oper.trim();

		// SPATIAL RELATIONSHIPS

		// Contains: HRR has at least 5 HSAs in it
		// Overlaps: HRR overlaps at least two Counties
		// Distance <=20: More than 2 hospitals within 20 mi from the Residence
		int count = this.getCountOfRelatedEntitiesOfType(relationship,
				entityType);
		try {
			int limit = Integer.parseInt(value);
			if ("<".equals(oper))
				return count < limit;
			else if ("<=".equals(oper))
				return count <= limit;
			else if ("==".equals(oper) || "=".equals(oper)
					|| "Is".equalsIgnoreCase(oper))
				return count == limit;
			else if (">=".equals(oper))
				return count >= limit;
			else if (">".equals(oper))
				return count > limit;
			else {
				Log.error("The relationship " + relationship
						+ " expects a comparison operator and not " + oper
						+ ". Condition assumed FALSE");
				return false;
			}

		} catch (Exception e) {
			Log.error("The relationship " + relationship
					+ " expects an integer Value: not " + value
					+ ". Condition assumed FALSE");
			return false;
		}

	}

	public int getCountOfRelatedEntitiesOfType(String relationship,
			String entityType) {
		long executionStart = System.currentTimeMillis();
		long distanceCalculationTime = 0;
		int n = 0;
		List<Geometry> list = GeoDatabase.getGeometryList(entityType);
		if (list == null) {
			return n;
		}

		String distanceCondition = null;
		String distanceOper = null;
		int distanceValue = -1;
		if (relationship.startsWith("Distance")) {
			try {
				distanceCondition = relationship.substring(9).trim();
				//Log.info("distanceCondition: " + distanceCondition);
				if (distanceCondition.startsWith("<=")
						|| distanceCondition.startsWith(">=")
						|| distanceCondition.startsWith("==")) {
					distanceOper = distanceCondition.substring(0, 2);
					distanceValue = Integer.parseInt(distanceCondition
							.substring(2));
				} else { // <, >, =
					distanceOper = distanceCondition.substring(0, 1);
					distanceValue = Integer.parseInt(distanceCondition
							.substring(1));
				}
			} catch (Exception e) {
				Log.info("Relationship 'Distance<condition>' " + relationship
						+ " does not have a valid condition");
				return 0;
			}
		}

		Geometry thisGeometry = getGeometry();
		for (int i = 0; i < list.size(); i++) {
			// Log.info("i=" + i + " n=" + n);
			Geometry iGeometry = list.get(i);
			if (relationship.equalsIgnoreCase("Contains")) {
				if (thisGeometry.contains(iGeometry))
					n++;
			} else if (relationship.equalsIgnoreCase("Overlaps")) {
				if (thisGeometry.overlaps(iGeometry))
					n++;
			} else if (relationship.startsWith("Distance")) {
				long distanceStart = System.currentTimeMillis();
				double d = thisGeometry.distance(iGeometry);
				long distanceTime = System.currentTimeMillis() - distanceStart;
				distanceCalculationTime += distanceTime;
				// if (n < 100) {
				// System.out.println("Distance calculation time: "
				// + distanceTime + " msec");
				// }
				// Log.info("i=" + i + " n=" + n);Log.info("  distance=" + d);
				if (distanceOper.equals("<=")) {
					if (d <= distanceValue)
						n++;
				} else if (distanceOper.equals("<")) {
					if (d < distanceValue)
						n++;
				} else if (distanceOper.equals("=")
						|| distanceOper.equals("==")
						|| distanceOper.equalsIgnoreCase("Is")) {
					if (Math.abs(d - distanceValue) < 0.001)
						n++;
				} else if (distanceOper.equals(">=")) {
					if (d >= distanceValue)
						n++;
				} else if (distanceOper.equals(">")) {
					if (d > distanceValue)
						n++;
				} else {
					Log.info("Relationship 'Distance<condition>' "
							+ relationship + " has invalid condition: "
							+ distanceOper + distanceValue);
					break;
				}
			} else {
				Log.info("Relationship " + relationship
						+ " is not supported yet");
				break;
			}
		}
		Log.info("Count: " + getType() + " " + getId() + " " + relationship
				+ " " + n + " " + entityType + "(s)");
		long executionTime = System.currentTimeMillis() - executionStart;
		Log.info("Count calculation time: " + executionTime + " msec");
		Log.info("Total distanceCalculationTime = " + distanceCalculationTime
				+ " msec");
		Log.info("Average distanceCalculationTime = " + distanceCalculationTime
				/ list.size() + " msec");
		return n;

	}

}
