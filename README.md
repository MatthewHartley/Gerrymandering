Gerrymandering

districting.cpp attempts to find convex districts that evenly distributes the
population, while having a target number of districts be majority Democrat.

Districts with too many people will attract districts with too few people,
tending to decrease the amount of inequality in the population of the districts.

If the desired party demographics do not hold, districts with high amounts of
partisanship will push other high partisanship districts orthogonally, causing
districts to rotate until desired district party demographics are met.