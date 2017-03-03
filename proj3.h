/**
 * @mainpage Project 3 - Simple cluster analysis
 * @author Martin Omacht (xomach00)
 * @date 11.12.2016
 *
 * This program uses method of complete linkage to make clusters of objects. Objects are loaded from text file.
 */

/**
 * @defgroup cluster Cluster
 * @brief functions for working with cluster
 *
 * @defgroup arrOfClusters Array Of Clusters
 * @brief functions for working with array of clusters
 *
 * @defgroup clusterAnalysis Cluster Analysis
 * @brief functions for cluster analysis
 */

/// Object structure
struct obj_t {
    int id;     ///< ID of the object
    float x;    ///< X coordinate of the object
    float y;    ///< Y coordinate of the object
};

/// Cluster structure
struct cluster_t {
    int size;           ///< number of elements in 'obj' array
    int capacity;       ///< number of elements the 'obj' array is allocated to
    struct obj_t *obj;  ///< array of objects
};

/**
 *
 * Initialization of cluster 'c'. Allocates memory for number of objects specified by 'cap'.
 *
 * @pre c != NULL
 * @pre cap >= 0
 *
 * @param c cluster to be initialized
 * @param cap capacitiy the cluster will be allocated to
 *
 * @ingroup cluster
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * Clear all objects from cluster and initialize to empty cluster.
 *
 * @pre c != NULL
 * @pre c->obj != NULL
 *
 * @post c->capacity == 0
 * @post c->obj == NULL
 *
 * @param c cluster to be cleared
 *
 * @ingroup cluster
 */
void clear_cluster(struct cluster_t *c);

/// Chunk of cluster objects. Value recommended for reallocation.
/// @ingroup cluster
extern const int CLUSTER_CHUNK;

/**
 * Resize cluster to a new capacity.
 *
 * @pre c != NULL
 * @pre c->capacity >= 0
 * @pre new_cap >= 0
 *
 * @param c cluster to be resized
 * @param new_cap new capacity of the cluster
 * @return 'c' or NULL if new capacity could not be allocated
 *
 * @ingroup cluster
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * Append object to cluster. Resizes cluster if needed.
 *
 * @pre c != NULL
 *
 * @param c cluster to append to
 * @param obj object to be appended
 *
 * @ingroup cluster
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * Add objects from cluster 'c2' to cluster 'c1'. Cluster 'c1' will be resized if needed.
 *
 * @pre c1 != NULL
 * @pre c1 != NULL
 *
 * @post cluster 'c1' will be sorted
 *
 * @param c1 cluster that will be merge into
 * @param c2 this cluster will merge into 'c1'
 *
 * @ingroup cluster
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Remove cluster from array of clusters.
 *
 * @pre idx >= 0
 * @pre idx < narr
 * @pre narr > 0
 *
 * @post last element of 'carr' is moved to index 'idx'
 *
 * @param carr array of clusters
 * @param narr size of 'carr'
 * @param idx index of the cluster that will be removed from 'carr'
 * @return new size of 'carr'
 *
 * @ingroup arrOfClusters
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * Calculate distance between two objects.
 *
 * @pre o1 != NULL
 * @pre o2 != NULL
 *
 * @param o1 pointer to object 1
 * @param o2 pointer to object 2
 * @return distance between the objects
 *
 * @ingroup clusterAnalysis
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * Calculate distance between two clusters using complete linkage method.
 *
 * @pre c1 != NULL
 * @pre c1->size > 0
 * @pre c2 != NULL
 * @pre c2->size > 0
 *
 * @param c1 pointer to cluster 1
 * @param c2 pointer to cluster 2
 * @return distance between the clusters
 *
 * @ingroup clusterAnalysis
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Find two closest clusters in array of clusters 'carr'. Indexes of the closest clusters will be stored in 'c1' and 'c2'.
 *
 * @pre narr > 0
 *
 * @param[in] carr array of clusters
 * @param[in] narr number of elements in 'carr'
 * @param[out] c1 index of the first neighbour cluster
 * @param[out] c2 index of the second neighbour cluster
 *
 * @ingroup clusterAnalysis
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * Sort objects in cluster by ID ascending.
 *
 * @param c cluster to be sorted
 *
 * @ingroup cluster
 */
void sort_cluster(struct cluster_t *c);

/**
 * Print attributes of all objects in a single cluster to stdout.
 *
 * @pre c != NULL
 *
 * @param c cluster to be printed
 *
 * @ingroup cluster
 */
void print_cluster(struct cluster_t *c);

/**
 * Load objects from file into an array of clusters. Each object is put into a separate cluster.
 *
 * @param[in] filename path to file to be loaded
 * @param[out] arr pointer to array of cluster to be loaded to
 * @return number of objects loaded
 *
 * @ingroup arrOfClusters
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * Print clusters from array of clusters to stdout. 'narr' is number of clusters to be prined.
 *
 * @param carr pointer to first cluster in array
 * @param narr number of clusters to be printed from 'carr'
 *
 * @ingroup arrOfClusters
 */
void print_clusters(struct cluster_t *carr, int narr);
