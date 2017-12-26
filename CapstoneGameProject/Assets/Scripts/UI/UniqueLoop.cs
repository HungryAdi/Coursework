using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// A basic data structure using a linked list and a dictionary to make what I call a UniqueLoop
// Features:
// Basically, T values are checked out by certain ids, and are no longer available for any other use
// Once the value is returned and a new value is requested, that value with be available again
public class UniqueLoop<T> {
    private LinkedList<T> ll; // internal linked list
    private LinkedListNode<T> head; // the first node of the linked list
    private Dictionary<int, LinkedListNode<T>> idDict; // dictionary for ids and their corresponding nodes

    // default constructor
    public UniqueLoop() {
        ll = new LinkedList<T>();
        idDict = new Dictionary<int, LinkedListNode<T>>();
    }

    // constructor passing an IEnumerable to LinkedList
    public UniqueLoop(IEnumerable<T> enumerable) {
        ll = new LinkedList<T>(enumerable);
        head = ll.First;
        idDict = new Dictionary<int, LinkedListNode<T>>();
    }

    // gets the next node that is not taken by any id
    private LinkedListNode<T> GetNextUnique(LinkedListNode<T> node) {
        LinkedListNode<T> current = node;
        LinkedListNode<T> previous = node.Previous ?? node.List.Last;
        while (current != previous) {
            current = current.Next ?? current.List.First;
            if (!idDict.ContainsValue(current)) {
                return current;
            }
        }
        return node;
    }

    // gets the previous node that is not taken by any id
    private LinkedListNode<T> GetPreviousUnique(LinkedListNode<T> node) {
        LinkedListNode<T> current = node;
        LinkedListNode<T> next = node.Next ?? node.List.First;
        while (current != next) {
            current = current.Previous ?? current.List.Last;
            if (!idDict.ContainsValue(current)) {
                return current;
            }
        }
        return node;
    }

    // gets the next item for the given id
    public T GetNextItem(int id) {
        if (idDict.ContainsKey(id)) {
            idDict[id] = GetNextUnique(idDict[id]);
        } else {
            idDict.Add(id, GetNextUnique(head));
        }
        return idDict[id].Value;
    }

    // gets the previous item for the given id
    public T GetPreviousItem(int id) {
        if (idDict.ContainsKey(id)) {
            idDict[id] = GetPreviousUnique(idDict[id]);
        } else {
            idDict.Add(id, GetPreviousUnique(head));
        }
        return idDict[id].Value;
    }

    // return the given id's item if it exists, returns T or default(T) otherwise
    public T ReturnItem(int id) {
        if (idDict.ContainsKey(id)) {
            T retVal = idDict[id].Value;
            idDict.Remove(id);
            return retVal;
        }
        return default(T);
    }
}
