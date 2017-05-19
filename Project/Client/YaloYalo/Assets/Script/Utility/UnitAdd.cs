using UnityEngine;

[ExecuteInEditMode]
public class UnitAdd : MonoBehaviour
{
    public string unitName;
    public bool isMagic;

    /* Available yaloCost
     * 1, 2, 3, 4, 5
     */
    public int yaloCost;

    /* unit prefab should contain below
     *  -unit model
     *  -unit script
     *  -hpBar if needed
     */
    public GameObject unitPrefab;

    /* unit prev should have value set below
     * - 340 x 340 sprite | ppu : 200
     */
    public Sprite unitPrevSprite;

    /* card sprite should have value set below
     * - 807 x 705 sprite | ppu : 75 
     */
    public Sprite cardSprite;
}