package main

import (
	"golang/dtree"
	"golang/utils/exp"
	"math/rand"
	"strconv"
)

func generateData(numRecords int) []map[string]string {
	names := []string{"Robinson", "Warner", "Stevens", "White", "Smith", "Green", "Brown", "Klaus", "Houston", "Long", "Short", "Doe"}
	roles := []string{"Developer", "Manager", "Analyst", "Clerk", "Salesperson"}
	educationLevels := []string{"Diploma", "Bachelor's", "Master's", "PhD"}
	performanceRatings := []string{"Below Average", "Average", "Above Average"}
	states := []string{"NJ", "FL", "CA", "TX", "NY"}
	maritalStatuses := []string{"Single", "Married"}
	salaryBands := []string{"Low", "Medium", "High"}
	genders := []string{"Male", "Female"}

	var dataRecords []map[string]string
	for i := 0; i < numRecords; i++ {
		record := make(map[string]string)

		record["name"] = names[rand.Intn(len(names))]
		record["age"] = strconv.Itoa(rand.Intn(40) + 21)
		record["gender"] = genders[rand.Intn(len(genders))]
		record["children"] = strconv.Itoa(rand.Intn(4))
		record["education_level"] = educationLevels[rand.Intn(len(educationLevels))]
		record["job_role"] = roles[rand.Intn(len(roles))]
		record["performance_rating"] = performanceRatings[rand.Intn(len(performanceRatings))]
		record["state"] = states[rand.Intn(len(states))]
		record["marital_status"] = maritalStatuses[rand.Intn(len(maritalStatuses))]
		record["salary_band"] = salaryBands[rand.Intn(len(salaryBands))]
		record["years_of_experience"] = strconv.Itoa(rand.Intn(39) + 1)
		dataRecords = append(dataRecords, record)
	}
	return dataRecords
}
func main() {
	dataRecords := generateData(100)
	attributes := []string{"age", "gender", "marital_status", "children", "education_level", "job_role", "years_of_experience", "performance_rating", "state"}
	targetAttribute := "salary_band"

	tree := dtree.BuildDecisionTree(dataRecords, attributes, targetAttribute)

	err := exp.ExportDecisionTreeToJson(tree, "complex_decision_tree.json")
	if err != nil {
		panic(err)
	}
}
